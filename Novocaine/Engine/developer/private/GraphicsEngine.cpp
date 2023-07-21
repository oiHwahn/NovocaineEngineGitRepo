#include "GraphicsEngine.h"
#include "SDL2/SDL.h"
#include "glew/glew.h"
#include "CoreMinimal.h"
#include "graphics/ShaderProgram.h"

// DEBUG INCLUDES
#include "graphics/ShapeMatrices.h" 
#include "graphics/Mesh.h"
#include "NovoTransform.h"

GraphicsEngine::GraphicsEngine()
{
	Window = nullptr;
	Renderer = nullptr;
	VCShader = nullptr;

	// debug 
	TriMesh = nullptr;
	PolyMesh = nullptr;
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
		900,
		900,
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
	// faill if it didnt work
	if (!VCShader->LoadShaders({ VShader, FShader })) {
		delete VCShader;
		NOVO_MSG_ERR("Graphics Engine", "Vetex colour shade failed.");

		return false;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// DEBUG INITIALISE MESHES
	PolyMesh = CreateShapeMesh(novosm::Polygon, VCShader);
	Poly2Mesh = CreateShapeMesh(novosm::Polygon, VCShader);
	TriMesh = CreateShapeMesh(novosm::Triangle, VCShader);
	Star1Mesh = CreateShapeMesh(novosm::Star1, VCShader);
	Star2Mesh = CreateShapeMesh(novosm::Star2, VCShader);

	// Circle Pieces
	Pizza1Mesh = CreateShapeMesh(novosm::Triangle, VCShader);
	Pizza2Mesh = CreateShapeMesh(novosm::Triangle, VCShader);
	Pizza3Mesh = CreateShapeMesh(novosm::Triangle, VCShader);
	Pizza4Mesh = CreateShapeMesh(novosm::Triangle, VCShader);

	Pizza5Mesh = CreateShapeMesh(novosm::Triangle2, VCShader);
	Pizza6Mesh = CreateShapeMesh(novosm::Triangle2, VCShader);
	Pizza7Mesh = CreateShapeMesh(novosm::Triangle2, VCShader);
	Pizza8Mesh = CreateShapeMesh(novosm::Triangle2, VCShader);
	
	// Geometric Shapes Repositions

	if (TriMesh != nullptr) {
		TriMesh->Transform.Location.x = 0.5f;
		TriMesh->Transform.Location.y = 0.5f;
		TriMesh->Transform.Scale = glm::vec3(0.15f);
	}

	if (PolyMesh != nullptr) {
		PolyMesh->Transform.Location.x = -0.5f;
		PolyMesh->Transform.Location.y = -0.5f;
		PolyMesh->Transform.Scale = glm::vec3(0.15f);
		PolyMesh->Transform.Rotation.z = 45.0f;
	}

	if (Poly2Mesh != nullptr) {
		Poly2Mesh->Transform.Location.x = -0.5f;
		Poly2Mesh->Transform.Location.y = 0.5f;
		Poly2Mesh->Transform.Scale = glm::vec3(0.15f);
		Poly2Mesh->Transform.Rotation.z = 45.0f;
	}

	if (Star1Mesh != nullptr) {
		Star1Mesh->Transform.Location.x = 0.5f;
		Star1Mesh->Transform.Location.y = -0.6f;
		Star1Mesh->Transform.Scale = glm::vec3(0.15f);
		Star1Mesh->Transform.Rotation.z = -125.0f;
	}

	if (Star2Mesh != nullptr) {
		Star2Mesh->Transform.Location.x = 0.47f;
		Star2Mesh->Transform.Location.y = -0.5f;
		Star2Mesh->Transform.Scale = glm::vec3(0.15f);
	}

	// Circle Repositions

	if (Pizza1Mesh != nullptr) {
		Pizza1Mesh->Transform.Location = glm::vec3(0.0f, 0.25f, 0.0f);
		Pizza1Mesh->Transform.Scale = glm::vec3(0.5f);
		Pizza1Mesh->Transform.Rotation.z = 180.0f;
	}

	if (Pizza2Mesh != nullptr) {
		Pizza2Mesh->Transform.Location = glm::vec3(0.0f, -0.25f, 0.0f);
		Pizza2Mesh->Transform.Scale = glm::vec3(0.5f);
	}

	if (Pizza3Mesh != nullptr) {
		Pizza3Mesh->Transform.Location = glm::vec3(0.25f, 0.0f, 0.0f);
		Pizza3Mesh->Transform.Scale = glm::vec3(0.5f);
		Pizza3Mesh->Transform.Rotation.z = 90.0f;

	}

	if (Pizza4Mesh != nullptr) {
		Pizza4Mesh->Transform.Location = glm::vec3(-0.25f, 0.0f, 0.0f);
		Pizza4Mesh->Transform.Scale = glm::vec3(0.5f);
		Pizza4Mesh->Transform.Rotation.z = -90.0f;

	}

	if (Pizza5Mesh != nullptr) {
		Pizza5Mesh->Transform.Location = glm::vec3(-0.19f, 0.19f, 0.0f);
		Pizza5Mesh->Transform.Scale = glm::vec3(0.36f, 0.52f, 0.0f);
		Pizza5Mesh->Transform.Rotation.z = -135.0f;
	}

	if (Pizza6Mesh != nullptr) {
		Pizza6Mesh->Transform.Location = glm::vec3(0.19f, -0.19f, 0.0f);
		Pizza6Mesh->Transform.Scale = glm::vec3(0.36f, 0.52f, 0.0f);
		Pizza6Mesh->Transform.Rotation.z = 45.0f;
	}

	if (Pizza7Mesh != nullptr) {
		Pizza7Mesh->Transform.Location = glm::vec3(-0.19f, -0.19f, 0.0f);
		Pizza7Mesh->Transform.Scale = glm::vec3(0.36f, 0.52f, 0.0f);
		Pizza7Mesh->Transform.Rotation.z = -45.0f;
	}

	if (Pizza8Mesh != nullptr) {
		Pizza8Mesh->Transform.Location = glm::vec3(0.19f, 0.19f, 0.0f);
		Pizza8Mesh->Transform.Scale = glm::vec3(0.36f, 0.52f, 0.0f);
		Pizza8Mesh->Transform.Rotation.z = 135.0f;
	}

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
	glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsEngine::DrawGraphics()
{
	// TODO: Draw 3d objects to the screen
	
	if (TriMesh != nullptr) {
		TriMesh->Transform.Rotation.z += 0.005f;
	}

	if (Poly2Mesh != nullptr) {
		Poly2Mesh->Transform.Rotation.z += 1.0f;
	}

	// loop through all of the meshstack elements
	for (Mesh* LMesh : MeshStack) {
		LMesh->Draw();
	}

}

void GraphicsEngine::PresentGraphics()
{
	// present the opengl renderer to the window
	SDL_GL_SwapWindow(Window);
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
 