#pragma once
#include "../Sampler2D.h"

class Sampler2DDX12 :
	public Sampler2D
{
public:

	GLuint wrapMap[2] = { GL_REPEAT, GL_CLAMP };
	GLuint filterMap[2] = { GL_NEAREST, GL_LINEAR };

	Sampler2DDX12();
	~Sampler2DDX12();
	void setMagFilter(FILTER filter);
	void setMinFilter(FILTER filter);
	void setWrap(WRAPPING s, WRAPPING t);

	GLuint magFilter, minFilter, wrapS, wrapT;
	GLuint samplerHandler = 0;
private:
};

