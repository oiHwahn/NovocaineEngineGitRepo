#pragma once
#include "CoreMinimal.h"
#include "glm/glm.hpp"


struct ShapeMatrices;

struct NovoVertex {
	// we assign a postion to the class variable on initialisation
	NovoVertex(glm::vec3 inposition, glm::vec3 incolour) : position(inposition), colour(incolour) {}

	glm::vec3 position; // x,y,z, position in world space of the vertex
	glm::vec3 colour;

	static TArray<NovoVertex> ConvertShapeMatrix(ShapeMatrices Shape);
};

class VertexArrayObject {
public:
	VertexArrayObject(const TArray<NovoVertex>& vertexData, const TArray<NovoUint>& indexData, const NovoUint RowSize = 6);
	~VertexArrayObject();

	// save the vertex data into a format that is readable by open GL shaders, that format is called an attribute pointer
	// @param 1 - index to assign this attribute pointer in the opengl attribute pointer array
	// @param 2 - how many number are in this pointer [e.g. 3 for a vertex position]
	// @param 3 - the data type that will be read for these number [e.g. float, int, etc...]
	// @param 4 - size of each row of numbers in bits [e.g. size of (datatype) * number]
	// @param 5 - amount of numbers to skip to get to the row that is in the vertext matrix [e.g. (void*)(sizeof(datatype)* number of elements)]
	void SetAttributePointer(NovoUint index, NovoUint size, NovoUint type, int stride, const void* pointer); 

	// activate this VAO in opengl
	void Bind();

	// deactivate this VAO
	void Unbind();

	// set up the IDs for this VAO
	void GenerateAndSetBuffers();
	
	// run every frame to draw it in the screen
	void Draw();

private:
	TArray<NovoVertex> VertexData;
	TArray<NovoUint> IndexData;


	// VAO ID - Vertex matrix ID - Index matrix ID
	NovoUint VaoID, VboID, EabID;
};