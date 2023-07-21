#pragma once
#include "CoreMinimal.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class Mesh;
class ShaderProgram;
struct ShapeMatrices;
class Texture;
class Camera;

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	// initialise all of the required grapics libraries
	bool Initialise();

	// return the open gl renderer context
	SDL_GLContext GetRenderer();

	// return the SDL window
	SDL_Window* GetWindow() const;

	// clear prev frame
	void ClearGraphics();

	// draw new frame
	void DrawGraphics();

	// present the new frame to the renderer
	void PresentGraphics();

	// any logic updates for the graphics engine
	void Update();

	// use a shape matrix to create a mesh
	Mesh* CreateShapeMesh(ShapeMatrices Shape, ShaderProgram* Shader);

	// create a 3d shape using the texture shader
	Mesh* Create3DShape(ShapeMatrices Shape);

	// get texture using filepath or create if not valid
	Texture* GetTexture(const char* FilePath);

	// Get the default engine texture	
	Texture* GetDefaultTexture() const { return DefaultEngineTexture; }

private: 

	// Initialize all the engine shaders
	bool InitEngineShaders();

	// Create the camera and screen transformations
	void ApplyWorldTransforms();

private:
	SDL_GLContext Renderer;
	SDL_Window* Window;

	// vertex colour shader (depracated)
	ShaderProgram* VCShader;
	// texture colour shader
	ShaderProgram* TexShader;

	// Default texture
	Texture* DefaultEngineTexture;

	// hold all of the textures loaded into the game
	TArray<Texture*> TextureStack;

	// Current rendering camera for the engine
	Camera* CurrentCamera;

	// DEBUG
	TArray<Mesh*> MeshStack;
	Mesh* CubeMesh;
}; 