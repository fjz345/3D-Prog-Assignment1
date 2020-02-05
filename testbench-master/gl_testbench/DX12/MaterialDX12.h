#pragma once
#include "../Material.h"
#include <GL/glew.h>
#include "ConstantBufferDX12.h"
#include <stdafx.h>

#include <vector>
#include <iterator>
#include <iostream>

class DX12Renderer;

#define DBOUTW( s )\
{\
std::wostringstream os_;\
os_ << s;\
OutputDebugStringW( os_.str().c_str() );\
}

#define DBOUT( s )\
{\
std::ostringstream os_;\
os_ << s;\
OutputDebugString( os_.str().c_str() );\
}

// use X = {Program or Shader}
#define INFO_OUT(S,X) { \
char buff[1024];\
memset(buff, 0, 1024);\
glGet##X##InfoLog(S, 1024, nullptr, buff);\
DBOUTW(buff);\
}

// use X = {Program or Shader}
#define COMPILE_LOG(S,X,OUT) { \
char buff[1024];\
memset(buff, 0, 1024);\
glGet##X##InfoLog(S, 1024, nullptr, buff);\
OUT=std::string(buff);\
}


class MaterialDX12 :
	public Material
{
	friend DX12Renderer;

public:
	MaterialDX12(const std::string& name);
	~MaterialDX12();

	void setShader(const std::string& shaderFileName, ShaderType type);
	void removeShader(ShaderType type);
	int compileMaterial(std::string& errString);
	int enable();
	void disable();
	void setDiffuse(Color c);

	// location identifies the constant buffer in a unique way
	void updateConstantBuffer(const void* data, size_t size, unsigned int location);
	// slower version using a string
	void addConstantBuffer(std::string name, unsigned int location);
	std::map<unsigned int, ConstantBufferDX12*> constantBuffers;

private:

	// TODO: change to PIPELINE
	// opengl program object
	std::string name;
	ID3DBlob* shaderBlobs[2]; // TYPE:VS/PS

	int compileShader(ShaderType type, std::string& errString);

	void SplitDefineString(std::string defineString, std::vector<D3D_SHADER_MACRO> &shaderMacro, std::vector<std::string> &vstrings);
};

