#include "GraphicsEngine.h"
#include "SDL2/SDL.h"
#include "glew/glew.h"
#include "CoreMinimal.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Camera.h"
#include "glm/gtc/matrix_transform.hpp"

// DEBUG INCLUDES
#include "graphics/ShapeMatrices.h" 
#include "graphics/Mesh.h"
#include "NovoTransform.h"
#include "graphics/Texture.h"

GraphicsEngine::GraphicsEngine()
{
	Window = nullptr;
	Renderer = nullptr;
	VCShader = nullptr;
	TexShader = nullptr;
	DefaultEngineTexture = nullptr;
	CurrentCamera = nullptr;

	// debug 
	CubeMesh = nullptr;

}

GraphicsEngine::~GraphicsEngine()
{
	SDL_DestroyWindow(Window);
	SDL_GL_DeleteContext(Renderer);
}

bool GraphicsEngine::Initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		NOVO_MSG_ERR("Graphics Engine", "SDL failed to initialise" << SDL_GetError());
		return false;
	}

	// set up open gl for SLD2
	// setting the required attributes for opengl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // major version of opengl to use v4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6); // minor version of opengl to use v4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY); // allow sdl to use the latest version
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // transparency
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // color red bit depth
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); // color green bit depth
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); // color blue bit depth

	// create the SDL flags for the window
	Uint32 WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

	Window = SDL_CreateWindow(
		"Novocaine Engine | An OpenGL Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		WindowFlags
	);

	// if the window wasnt successfully created then fail
	if (Window == nullptr) {
		NOVO_MSG_ERR("Graphics Engine", "SDL failed to create window: " << SDL_GetError());
		SDL_Quit();
		return false;
	}

	// create the opengl renderer
	Renderer = SDL_GL_CreateContext(Window);

	// if the renderer failed to create then fail
	if (Renderer == nullptr) {
		NOVO_MSG_ERR("Graphics Engine", "SDL failed to create opengl context: " << SDL_GetError());
		SDL_DestroyWindow(Window);
		SDL_Quit();
		return false;
	}
	
	// activates the experimental libraries in GLEW (which seems to be all of them?)
	glewExperimental = GL_TRUE;

	// initialise glew and fail if it doesnt succeed
	if (glewInit() != GLEW_OK) {
		NOVO_MSG_ERR("Graphics Engine", "GLEW failed to initialise: " << glewGetErrorString(glewInit()));
		SDL_GL_DeleteContext(Renderer);
		SDL_DestroyWindow(Window);
		SDL_Quit();
		return false;

	}

	// Activate the ability to read depth in OpenGL
	glEnable(GL_DEPTH_TEST);

	// Initialise the engine shaders
	InitEngineShaders();

	// Create a default camera
	CurrentCamera = new Camera(0.0f, 0.0f, 0.0f);

	DefaultEngineTexture = GetTexture("Engine/developer/textures/T_NovoDefaultTexture.png");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   
	return true;
}

SDL_GLContext GraphicsEngine::GetRenderer()
{
	return Renderer;
}

SDL_Window* GraphicsEngine::GetWindow() const
{
	return Window;
}

void GraphicsEngine::ClearGraphics()
{
	// set the bg colour when there are no objects over it
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::DrawGraphics()
{
	// TODO: Draw 3d objects to the screen

	// loop through all of the meshstack elements
	for (Mesh* LMesh : MeshStack) {
		LMesh->Draw();
	}

	ApplyWorldTransforms();
}

void GraphicsEngine::PresentGraphics()
{
	// present the opengl renderer to the window
	SDL_GL_SwapWindow(Window);
}

void GraphicsEngine::Update()
{
	CurrentCamera->Update();
}

Mesh* GraphicsEngine::CreateShapeMesh(ShapeMatrices Shape, ShaderProgram* Shader)
{
	Mesh* NewMesh = new Mesh(Shader);

	

	if (!NewMesh->InitialiseVAO(Shape)) {
		NOVO_MSG_ERR("GE", "Mesh VAO could not be cread.");

		return nullptr;
	}

	// add our new mesh into the mesh stack
	MeshStack.push_back(NewMesh);

	return NewMesh;
}

Mesh* GraphicsEngine::Create3DShape(ShapeMatrices Shape)
{
	return CreateShapeMesh(Shape, TexShader);
}

Texture* GraphicsEngine::GetTexture(const char* FilePath)
{
	// loop through the textures and see if one already exist with the same path
	for (Texture* LTexture : TextureStack) {
		// if the filepath is the same then just exit the function and return the texture
		// strcmp is the better way to constant char* of strings and returns 0 if its a match
		if (strcmp(LTexture->GetFilePath(), FilePath) == 0) {
			return LTexture;
		}
	}

	// if there is no texture found then create a new one
	Texture* NewTexture = new Texture();

	// import the texture but delete it if it doesnt work
	if (!NewTexture->ImportTexture(FilePath)) {
		delete NewTexture;
		return nullptr;
	}

	// if all is successful, add the texture to the texture stack to make sure we dont get another
	TextureStack.push_back(NewTexture);

	return NewTexture;
} 


bool GraphicsEngine::InitEngineShaders()
{	
	// create the vc shader
	VCShader = new ShaderProgram();

	// importing the shaders from their files
	//vertex shader
	NovoShaderInfo VShader(
		NovoShaderTypes::VERTEX,
		"Engine/developer/shaders/VColour/VColour.novovshader"
	);

	// frament shader
	NovoShaderInfo FShader(
		NovoShaderTypes::FRAGMENT,
		"Engine/developer/shaders/VColour/VColour.novofshader"
	);

	// load the shader
	// fail if it didnt work
	if (!VCShader->LoadShaders({ VShader, FShader })) {
		delete VCShader;
		NOVO_MSG_ERR("Graphics Engine", "Vertex colour shader failed.");

		return false;
	}
	//////////////////////////////////////////////
	TexShader = new ShaderProgram();

	// import the vertex shader
	VShader = NovoShaderInfo(
		NovoShaderTypes::VERTEX,
		"Engine/developer/shaders/Texture/Texture.novovshader"
	);

	// import the fragment shader
	FShader = NovoShaderInfo(
		NovoShaderTypes::FRAGMENT,
		"Engine/developer/shaders/Texture/Texture.novofshader" 
	);

	if (!TexShader->LoadShaders({VShader, FShader})) {
		delete TexShader;
		NOVO_MSG_ERR("Graphics Engine", "Texture Shader failed");

		return false;
	}

	return true;
}

void GraphicsEngine::ApplyWorldTransforms()
{
	// hold the width and height of the window
	int WWidth, WHeight = 0;

	// Get the window and change our width and height variables to the screen size
	SDL_GetWindowSize(GetWindow(), &WWidth, &WHeight);

	// convert the screen size into and aspect ration
	float AspectRatio = static_cast<float>(WWidth) / static_cast<float>(std::max(WHeight, 1));

	// create the view and projection transforms
	glm::mat4 view(1.0f); // view = glm::mat4(1.0f);
	glm::mat4 projection(1.0f);

	// create the view coordinates based on the camera location
	// LookAt params
	// @param1 - Camera Location
	// @param2 - Camera Focus (Where camera should look)
	// @param3 - Camera local up vector
	view = glm::lookAt(
		CurrentCamera->Transform.Location,
		CurrentCamera->Transform.Location + CurrentCamera->Transform.GetForward(),
		CurrentCamera->Transform.GetUp()
	);

	// create the world coordinates from the screen
	// otherwise known as perspective view
	projection = glm::perspective(
		glm::radians(CurrentCamera->FOV),
		AspectRatio,
		CurrentCamera->NearClip,
		CurrentCamera->FarClip
	);

	// add the coordinates to the texture shader
	TexShader->SetUniformTransform("View", view);
	TexShader->SetUniformTransform("Projection", projection);

}
 