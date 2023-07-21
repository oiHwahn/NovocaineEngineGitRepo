#include "graphics/Texture.h"
#include "glew/glew.h"
#define STB_IMAGE_IMPLEMENTATION  
#include "stbimage/stb_image.h"

Texture::Texture()
{
	ID = 0;
	FilePath = "";
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

bool Texture::ImportTexture(const char* FilePath)
{
	this->FilePath = FilePath;

	// Create the variables to store the image data
	int Width, Height, nChannels;
	// stbi image importas images upside down
	// so we need to run this function to load them in correctly
	stbi_set_flip_vertically_on_load(true);
	// load in the texture and fill in our data
	unsigned char* ImageData = stbi_load(FilePath, &Width, &Height, &nChannels, STBI_rgb_alpha);  
	
	// if the image data didnt load then fail the function and log the error
	if (ImageData == nullptr) {
		NOVO_MSG_ERR("Texture", "Texture" << FilePath << "Failed to import.");
		stbi_image_free(ImageData);

		return false;
	}

	// generate an ID for the texture
	glGenTextures(1, &ID);
	
	// Make sure the ID was assigned
	if (ID == GL_INVALID_INDEX) {
		NOVO_MSG_ERR("Texture", "Texture" << FilePath << "Failed to assign an ID in opengl.");
		stbi_image_free(ImageData);

		return false;
	}

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, ID);

	// Set the parameters of the texture 
	// what happens when the texture is larger or smaller than the map size
	// setting what happens when the texture doesnt fit the x axis (S = X in textures)
	// it should repeat or copy itself over and over until it fits
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// do the same but for the Y axis (T = Y in textures)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// what happens if the texture resolution is scaled up and down
	// MIN_FILTER is when the texture scales down and MAG is scale up
	// Linear interpolates colours to create a blend to change the pixels
	// NEAREST copies the pixel next to it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// create the texture information an store it in open GL
	// p1 = Type of texture
	// P2 = irrelevant
	// p3 = format to read the texture file
	// p4 & 5 = width and height of the image
	// p6 = if we want a border (not for textures)
	// p7 = format to conver the file to
	// p8 = the type of data to read (this is set by stbi image)
	// p9 = the stored image data to read
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);

	// removes image data from memory
	stbi_image_free(ImageData);

	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR) {
		NOVO_MSG_ERR("Texture", "Texture" << FilePath << "Failed to generate texture with error code: " << Error);
		 
		return false;
	}

	NOVO_MSG_LOG("Texture", FilePath << " was Loaded Successfully.")

	// set the default texture as the current bound texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::Bind(NovoUint TextureSlot)
{
	// creates a texture unit for this texture to bind to
	glActiveTexture(TextureSlot);
	glBindTexture(GL_TEXTURE_2D, GetID());
}
