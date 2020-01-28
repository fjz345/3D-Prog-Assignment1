#pragma once
#include <GL/glew.h>
#include "../VertexBuffer.h"
#include <stdafx.h>

class VertexBufferDX12 :
	public VertexBuffer
{
public:
	VertexBufferDX12(size_t size, VertexBuffer::DATA_USAGE usage);
	~VertexBufferDX12();
	
	void setData(const void* data, size_t size, size_t offset);
	void bind(size_t offset, size_t size, unsigned int location);
	void unbind();
	size_t getSize();

	ID3D12Resource1** getVertexBufferResource();
	D3D12_VERTEX_BUFFER_VIEW* getVertexBufferView();

private:
	ID3D12Resource1* vertexBufferResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	size_t totalSize;
};

