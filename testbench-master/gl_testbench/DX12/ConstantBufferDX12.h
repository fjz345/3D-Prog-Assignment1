#pragma once
#include <GL/glew.h>
#include "../ConstantBuffer.h"
#include "stdafx.h"
#include "DX12Renderer.h" // Endast för NUM_SWAP_BUFFERS!

class ConstantBufferDX12 : public ConstantBuffer
{
public:
	ConstantBufferDX12(std::string NAME, unsigned int location);
	~ConstantBufferDX12();
	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material*);

	ID3D12Resource1** getConstantBufferResource();

	struct xyzw
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct rgba
	{
		float r;
		float g;
		float b;
		float a;
	};

	struct cbufferFloat4
	{
		union
		{
			xyzw position;
			rgba color;
		};
	};

private:
	std::string name;
	ID3D12Resource1* constantBufferResource[NUM_SWAP_BUFFERS] = {};
	cbufferFloat4 constantBufferCPU = {};
};

