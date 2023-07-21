#pragma once
#include "CoreMinimal.h"

struct ShapeMatrices {

	ShapeMatrices(TArray<float> data, TArray<NovoUint> indices, NovoUint rowsize)
		: Data(data), Indices(indices), RowSize(rowsize) {}

	TArray<float> Data;
	TArray<NovoUint> Indices;
	NovoUint RowSize;
};

namespace novosm {
	static const ShapeMatrices Triangle(
		{
			// x	// y	// z	// colour rgb
			 0.0f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	// 0 - top of the triangle - red	
			-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	// 1 - bottom left - green
			 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f	// 2 - bottom right - blue
		},
		{
			0, 1, 2		// triangle 1
		},
		6
	);

	static const ShapeMatrices Triangle2(
		{
			// x	// y	// z	// colour rgb
			 0.0f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// 0 - top of the triangle - red	
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// 1 - bottom left - green
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f	// 2 - bottom right - blue
		},
		{
			0, 1, 2		// triangle 2
		},
		6
	);


	static const ShapeMatrices Polygon(
		{
			// x	// y	// z	// colour rgb
			-0.5f,  0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	// 0 - top left - yellow 
			 0.5f,  0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	// 1 - top left - yellow 
			-0.5f, -0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	// 2 - bottom left - yellow 
			 0.5f, -0.5f,	0.0f,	1.0f, 1.0f, 0.0f	// 3 - bottom right - yellow
		},
		{
			0, 1, 2, 		// triangle 1
			1, 2, 3		// triangle 2 
		},
		6
	);

	// Star Shape

	static const ShapeMatrices Star1(
		{
			// x	 // y	// z		// colour rgb
			0.20f,	 1.0f,	0.0f,		1.0f, 0.0f, 1.0f,		
			0.0f,	-0.25f,	0.0f,		1.0f, 0.0f, 1.0f,		
			-1.0f,	-1.0f,	0.0f,		1.0f, 0.0f, 1.0f,		
			1.25f,	-0.5f,	0.0,		1.0f, 0.0f, 1.0f		
		},
		{
			0, 1, 2, 		// triangle 1
			1, 2, 3		// triangle 2 
		},
		6
	);

	static const ShapeMatrices Star2(
		{
			// x	// y	// z	// colour rgb
			 1.25f,  0.0f, 0.0f,		1.0f, 0.0f, 1.0f,		
			-1.25f,  0.0f, 0.0f,		1.0f, 0.0f, 1.0f,	
			 0.0f,  -0.5f, 0.0f,		1.0f, 0.0f, 1.0f	
		},
		{
			0, 1, 2		// triangle 2
		},
		6
	);
}
