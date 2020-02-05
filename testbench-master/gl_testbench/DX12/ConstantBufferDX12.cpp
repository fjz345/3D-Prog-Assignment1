#include "ConstantBufferDX12.h"

ConstantBufferDX12::ConstantBufferDX12(std::string NAME, unsigned int location)
{
	name = NAME;
}

ConstantBufferDX12::~ConstantBufferDX12()
{
	
}

// this allows us to not know in advance the type of the receiving end, vec3, vec4, etc.
void ConstantBufferDX12::setData(const void* data, size_t size, Material* m, unsigned int location)
{
	void* dataBegin = nullptr;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.
	// TODO: Behövs detta göras här? Vi gör detta sedan dynamiskt
	// Vad behövs sättas upp här
	constantBufferResource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, data, size); // Sets all data to 0
	constantBufferResource->Unmap(0, nullptr);
}

void ConstantBufferDX12::bind(Material* m)
{

}

ID3D12Resource1** ConstantBufferDX12::getConstantBufferResource()
{
	return &constantBufferResource;
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

