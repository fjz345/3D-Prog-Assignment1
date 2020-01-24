#include "ConstantBufferDX12.h"

ConstantBufferDX12::ConstantBufferDX12(std::string NAME, unsigned int location)
{
	
}

ConstantBufferDX12::~ConstantBufferDX12()
{
	
}

// this allows us to not know in advance the type of the receiving end, vec3, vec4, etc.
void ConstantBufferDX12::setData(const void* data, size_t size, Material* m, unsigned int location)
{
	
}

void ConstantBufferDX12::bind(Material* m)
{
	
}

/*
	//if (_index = GL_MAX_UNIFORM_LOCATIONS)
	//{
	//	MaterialGL* material = (MaterialGL*)m;
	//	_index = glGetUniformLocation(material->getProgram(), "translate");
	//}
	//if (_index == 0)
	// _index = glGetUniformBlockIndex(material->getProgram(), "transform");
	//glUniformBlockBinding(material->getProgram(), _index, _location);
	//glUniform4fv(_location, 1, (float*)_buff);
}
*/

