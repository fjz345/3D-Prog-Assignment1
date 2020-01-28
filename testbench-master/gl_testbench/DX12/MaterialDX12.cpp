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
	name = _name;
}

MaterialDX12::~MaterialDX12()
{ 
	
}

void MaterialDX12::setDiffuse(Color c)
{
	
}

void MaterialDX12::setShader(const std::string& shaderFileName, ShaderType type)
{
	shaderFileNames[type] = shaderFileName;
}

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
	////// Shader Compiles //////
	shaderBlobs[int(type)] = nullptr;

	std::wstring tempShaderName = std::wstring(shaderFileNames[type].begin(), shaderFileNames[type].end());
	LPCWSTR name = tempShaderName.c_str();

	std::string entryPoint;
	std::string shaderModelTarget;
	if (type == Material::ShaderType::VS)
	{
		entryPoint = "VS_main";
		shaderModelTarget = "vs_5_0";
	}
	else if(type == Material::ShaderType::PS)
	{
		entryPoint = "PS_main";
		shaderModelTarget = "ps_5_0";
	}

	
	auto it = shaderDefines[type].begin();
	std::string defines = (*it);

	// TODO: Fixa macro
	D3D_SHADER_MACRO shaderMacro;
	shaderMacro.Name = defines.c_str();
	shaderMacro.Definition = "";

	ID3DBlob* errorMessages = nullptr;
	
	auto hr = D3DCompileFromFile(
		name, // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		entryPoint.c_str(),		// entry point
		shaderModelTarget.c_str(),		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&shaderBlobs[int(type)],	// double pointer to ID3DBlob		
		&errorMessages			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	if (FAILED(hr) && errorMessages)
	{
		const char* errorMsg = (const char*)errorMessages->GetBufferPointer();
		// Printa i output
		OutputDebugStringA(errorMsg);
	}

	return 0;
}

int MaterialDX12::compileMaterial(std::string& errString)
{
	// Compile Vertex shader
	compileShader(Material::ShaderType::VS, errString);

	// Compile Pixel shader
	compileShader(Material::ShaderType::PS, errString);

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
