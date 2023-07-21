#pragma once
#include "CoreMinimal.h"
#include "NovoTransform.h"


struct NovoVertex; 
struct NovoTransform;
class VertexArrayObject;
class ShaderProgram;
struct ShapeMatrices;

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


private:
	// Store the VAO
	VertexArrayObject* VAO;
	// Store the shader
	ShaderProgram* Shader;

};