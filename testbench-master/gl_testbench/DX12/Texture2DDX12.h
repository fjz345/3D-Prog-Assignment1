#pragma once

#include <GL/glew.h>

#include "../Texture2D.h"
#include "Sampler2DDX12.h"


class Texture2DDX12 :
	public Texture2D
{
public:
	Texture2DDX12();
	~Texture2DDX12();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);

	// OPENGL HANDLE
	GLuint textureHandle = 0;
};

