#pragma once
#include "CoreMinimal.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class Mesh;
class ShaderProgram;
struct ShapeMatrices;

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

	// use a shape matrix to create a mesh
	Mesh* CreateShapeMesh(ShapeMatrices Shape,ShaderProgram* Shader);

private:
	SDL_GLContext Renderer;
	SDL_Window* Window;

	ShaderProgram* VCShader;

	// DEBUG
	TArray<Mesh*> MeshStack;
	Mesh* PolyMesh;
	Mesh* Poly2Mesh;
	Mesh* TriMesh;

	Mesh* Star1Mesh;
	Mesh* Star2Mesh;

	Mesh* Pizza1Mesh;
	Mesh* Pizza2Mesh;
	Mesh* Pizza3Mesh;
	Mesh* Pizza4Mesh;
	Mesh* Pizza5Mesh;
	Mesh* Pizza6Mesh;
	Mesh* Pizza7Mesh;
	Mesh* Pizza8Mesh;



}; 