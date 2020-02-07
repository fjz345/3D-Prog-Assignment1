#pragma once

#include <GL/glew.h>

#include "../Texture2D.h"
#include "Sampler2DDX12.h"
#include <wincodec.h>
#include "stdafx.h"
#include "DX12Renderer.h"

class Texture2DDX12 :
	public Texture2D
{
public:
	Texture2DDX12(DX12Renderer* renderer);
	~Texture2DDX12();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);

private:
	int loadTextureFromFile(std::string filename);

	BYTE* imageData = nullptr;
	int bytesPerRow;

	D3D12_RESOURCE_DESC resourceDescription;
	ID3D12Resource* textureBuffer;
	ID3D12Resource* textureBufferUploadHeap;

	DX12Renderer* renderer;
	ID3D12Device5* device;
};

