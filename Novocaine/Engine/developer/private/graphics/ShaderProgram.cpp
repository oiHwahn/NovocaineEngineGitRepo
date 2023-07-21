#include "graphics/ShaderProgram.h"
#include "glew/glew.h"
#include "glm/gtc/type_ptr.hpp"
// give us the ability to convert text file into a string
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram()
	: ProgramID(0)
{

}

ShaderProgram::~ShaderProgram()
{
	// remove the program
	if (ProgramID != 0) {
		glDeleteProgram(ProgramID);
	}

	// remove the shaders
	for (NovoUint ShaderID : ShaderIDs) {
		glDeleteShader(ShaderID);
	}
}

bool ShaderProgram::LoadShaders(const TArray<NovoShaderInfo>& Shaders)
{
	// generate an ID for the shader program
	ProgramID = glCreateProgram();

	// singular version of the array item : the array itself 
	// will run for the amount of items in the array
	for (const NovoShaderInfo& Shader : Shaders) {
		NovoUint ShaderID = CreateShader(Shader.ShaderType, Shader.FilePath);

		// not returning an error as the create shader function handles that
		if (ShaderID == 0) {
			return false;
		}

		NOVO_MSG_LOG("Shader Program", "Shader loaded: " << Shader.FilePath);
		
		// in open gl attach the shader to the relevant progroam
		glAttachShader(ProgramID, ShaderID);
	}

	// compile the program
	glLinkProgram(ProgramID);

	// error check if linking failed
	// see the similar code in CreateShader for comments
	int LinkStatus = 0; 
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &LinkStatus);
	 
	if (LinkStatus == GL_FALSE) {
		int InfoLogLength = 0; 
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		NovoString InfoLog(InfoLogLength, ' ');
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &InfoLog[0]);

		NOVO_MSG_ERR("Shader Program", "Shader linking error:\n" << InfoLog);

		glDeleteProgram(ProgramID);
		ProgramID = 0;
		return false;
	}

	return true;
}

void ShaderProgram::Run() const
{
	if (ProgramID != 0) {
		glUseProgram(ProgramID);
	}
}

void ShaderProgram::SetUniformTransform(glm::mat4 Transform)
{
	// find the uniform variable in the shader by name
	int UniformLoc = glGetUniformLocation(ProgramID, "Transform");

	// We change the value
	glUniformMatrix4fv(UniformLoc, 1, GL_FALSE, value_ptr(Transform));
}

NovoUint ShaderProgram::CreateShader(NovoShaderTypes ShaderType, const NovoString& FilePath)
{
	NovoString ShaderCode = ConvertShaderFile(FilePath);

	// make sure the file exists
	if (ShaderCode.empty()) {
		NOVO_MSG_ERR("ShaderProgram", "Failed to read shader file: " << FilePath);
		return 0;
	}

	// find the shader type and get the correct open gl enum value
	GLenum GLShaderType = GL_INVALID_ENUM;
	switch (ShaderType) {
	case VERTEX:
		GLShaderType = GL_VERTEX_SHADER;
		break;
	case FRAGMENT:
		GLShaderType = GL_FRAGMENT_SHADER;
		break;
	default:
		GLShaderType = GL_INVALID_ENUM;
		break;
	}

	// create the relevant shader
	// generate the shader id based on the type of shader
	NovoUint ShaderID = glCreateShader(GLShaderType);

	// convert code into const char* format so glew can read the text
	const char* ShaderCodeChar = ShaderCode.c_str();

	// creating the open gl version of the shader code
	glShaderSource(ShaderID, 1, &ShaderCodeChar, nullptr);

	// compile the shader and error if anything is wrong
	// compiling basically means to check for problems
	glCompileShader(ShaderID);

	// error handle any issues with the compile
	int CompileStatus = 0; 
	// update our local variable with the compile status
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &CompileStatus);

	// if the compile failed the it will return GL_FALSE
	if (CompileStatus == GL_FALSE) {
		// get the length of the compile error
		// how many characters are in the error
		int InfoLogLength = 0;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		// create empty string with the size of the log length
		NovoString CompilerMsg(InfoLogLength, ' ');
		// get the message and add it into the above variable
		glGetShaderInfoLog(ShaderID, InfoLogLength, nullptr, &CompilerMsg[0]);

		NOVO_MSG_ERR("Shader Program", "Shader compilation message: \n" << CompilerMsg);
		glDeleteShader(ShaderID);

		return 0;

	}

	// adds the shader into the program shaders array
	ShaderIDs.push_back(ShaderID);

	return ShaderID;
}

NovoString ShaderProgram::ConvertShaderFile(const NovoString FilePath)
{
	// ifstream type stroes file paths as actualpaths
	std::ifstream ShaderFile(FilePath);

	std::stringstream ShaderStream;
	// read the file path and add the text inside the file into a string
	ShaderStream << ShaderFile.rdbuf();

	// convert into a C++ string
	NovoString ReadableString(ShaderStream.str());

	return ReadableString;
}
