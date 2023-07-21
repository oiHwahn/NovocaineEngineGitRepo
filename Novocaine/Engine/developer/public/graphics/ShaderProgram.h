#pragma once
#include "CoreMinimal.h"
#include "glm/glm.hpp"

enum NovoShaderTypes : NovoUint {
	VERTEX = 0,
	FRAGMENT
};

struct NovoShaderInfo {
	NovoShaderInfo(NovoShaderTypes InType, NovoString InPath)
		: ShaderType (InType), FilePath(InPath) {}

	NovoShaderTypes ShaderType;
	NovoString FilePath;
};

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	// Initialize the shader files
	// vertex shader should be added first then the fragment shader 
	bool LoadShaders(const TArray<NovoShaderInfo>& Shaders);

	// Activate the shader as the current shader program for Open GL
	void Run() const;

	// Return the open gl ID for that shader 
	NovoUint GetID() const { return ProgramID; }

	// this will change the "Transform" shader variable
	void SetUniformTransform(glm::mat4 Transform);

private:
	// creates the individual shaders based on their type
	// vertex and frag shaders will be assigned seperately
	// returns 0 when it fails
	NovoUint CreateShader(NovoShaderTypes ShaderType, const NovoString& FilePath);

	// read the GLSL format and conver it into a string for open gl
	NovoString ConvertShaderFile(const NovoString Filepath);

private:
	NovoUint ProgramID;
	TArray<NovoUint> ShaderIDs;

};