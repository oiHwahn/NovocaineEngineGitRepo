#pragma once
#include "CoreMinimal.h"

class Texture {
public: 
	Texture();
	~Texture();

	// import a texture file from the file path location and save it to openGL
	bool ImportTexture(const char* FilePath);

	// Activate the texture for use in open GL
	void Bind(NovoUint TextureSlot);

	// Get the ID location of the texture in opengl
	NovoUint GetID() const { return ID; }

	const char* GetFilePath() const { return FilePath; }

private:
	// ID location of the texture in opengl
	NovoUint ID;

	// Use this to test if texture is already loaded
	const char* FilePath;
};