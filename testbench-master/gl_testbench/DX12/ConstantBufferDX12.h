#pragma once
#include <GL/glew.h>
#include "../ConstantBuffer.h"

class ConstantBufferDX12 : public ConstantBuffer
{
public:
	ConstantBufferDX12(std::string NAME, unsigned int location);
	~ConstantBufferDX12();
	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material*);

private:

	std::string name;
	GLuint location;
	GLuint handle;
	GLuint index;
	void* buff = nullptr;
	void* lastMat;
};

