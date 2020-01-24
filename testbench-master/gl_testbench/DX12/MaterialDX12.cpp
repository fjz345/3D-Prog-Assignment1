#include <windows.h>
#include <streambuf>
#include <sstream>
#include <istream>
#include <fstream>
#include <vector>
#include <set>
#include <assert.h>

#include "MaterialDX12.h"

typedef unsigned int uint;

MaterialDX12::MaterialDX12(const std::string& _name)
{ 
	
};

MaterialDX12::~MaterialDX12()
{ 
	
};

void MaterialDX12::setDiffuse(Color c)
{
	
}

void MaterialDX12::setShader(const std::string& shaderFileName, ShaderType type)
{
	
};

// this constant buffer will be bound every time we bind the material
void MaterialDX12::addConstantBuffer(std::string name, unsigned int location)
{
	
}

// location identifies the constant buffer in a unique way
void MaterialDX12::updateConstantBuffer(const void* data, size_t size, unsigned int location)
{
	
}

void MaterialDX12::removeShader(ShaderType type)
{
	
};

int MaterialDX12::compileShader(ShaderType type, std::string& errString)
{
	return 0;
}

int MaterialDX12::compileMaterial(std::string& errString)
{
	return 0;
};

int MaterialDX12::enable() {
	return 0;
};

void MaterialDX12::disable() {
	
};

//int MaterialGL::updateAttribute(
//	ShaderType type,
//	std::string &attribName,
//	void* data,
//	unsigned int size)
//{
//	return 0;
//}
