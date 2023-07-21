#pragma once
#include "CoreMinimal.h"
#include "NovoTransform.h"

struct NovoVertex; 
struct NovoTransform;
class VertexArrayObject;
class ShaderProgram;
struct ShapeMatrices;
class Texture;

class Mesh {
public:
	Mesh(ShaderProgram* Shader);
	~Mesh();

	// turn the mesh into a VAO mesh
	bool InitialiseVAO(ShapeMatrices Shape);

	// draw all the required draws for the mesh
	// shader
	// VAO
	void Draw();

	// store and change the transformation of the mesh
	NovoTransform Transform;

	// debug texture for testing simple shader
	Texture* BaseColour;


private:
	// Store the VAO
	VertexArrayObject* VAO;
	// Store the shader
	ShaderProgram* Shader;

};