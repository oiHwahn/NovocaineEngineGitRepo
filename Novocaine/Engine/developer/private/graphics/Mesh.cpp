#include "graphics/Mesh.h"
#include "graphics/VertexArrayObject.h"
#include "graphics/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "graphics/ShapeMatrices.h"

Mesh::Mesh(ShaderProgram* Shader) : Shader(Shader)
{
	Shader = nullptr;
	VAO = nullptr;

}

Mesh::~Mesh()
{
	if (VAO != nullptr) {
		delete VAO;
	}
}

bool Mesh::InitialiseVAO(ShapeMatrices Shape)
{
	// Convert the shape data into an array of vertices
	TArray<NovoVertex> ShapeData = NovoVertex::ConvertShapeMatrix(Shape);
	
	// Create the vao using the shape 
	VAO = new VertexArrayObject(ShapeData, Shape.Indices, Shape.RowSize);
	
	// if it was successful return true
	return VAO != nullptr;
}

void Mesh::Draw()
{
	// set the shader for this mesh as the active shader
	Shader->Run();

	glm::mat4 ShaderTransform = glm::mat4(1.0f);

	ShaderTransform = glm::translate(ShaderTransform, Transform.Location);
	
	// handle rotations
	// rotate the x axis based on the x value in rotation
	ShaderTransform = glm::rotate(ShaderTransform, glm::radians(Transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	// rotate the y axis based on the y value in rotation
	ShaderTransform = glm::rotate(ShaderTransform, glm::radians(Transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	// rotate the z axis based on the z value in rotation
	ShaderTransform = glm::rotate(ShaderTransform, glm::radians(Transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	// scale the mesh base on the transform scale 
	ShaderTransform = glm::scale(ShaderTransform, glm::vec3(Transform.Scale));

	// update the shader with the new transform
	Shader->SetUniformTransform(ShaderTransform);

	//Draw the VAO to the screen
	VAO->Draw(); 
}
 