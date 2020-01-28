#include "VertexBufferDX12.h"
#include "MeshDX12.h"
#include <assert.h>

/*
	Create a VertexBuffer backed in gpu mem
	- todo: add error checking...
*/
VertexBufferDX12::VertexBufferDX12(size_t size, DATA_USAGE usage)
{
	

	


}

VertexBufferDX12::~VertexBufferDX12()
{
	
}

/*
	SSBO based Vertex Buffer
*/
void VertexBufferDX12::setData(const void* data, size_t size,  size_t offset)
{
	// check so that we are inside totalsize
	if (size + offset > totalSize)
		return;

	void* dataBegin = (char*)nullptr + offset;

	// Set up the heap data
	D3D12_RANGE range = { 0, 0 }; // We do not intend to read this resource on the CPU.
	// TODO: Behövs detta göras här? Vi gör detta sedan dynamiskt
	// Vad behövs sättas upp här
	vertexBufferResource->Map(0, &range, &dataBegin); // Get a dataBegin pointer where we can copy data to
	memcpy(dataBegin, data, size); // Sets all data to 0
	vertexBufferResource->Unmap(0, nullptr);
}

/*
 bind at "location", with offset "offset", "size" bytes 
 */
void VertexBufferDX12::bind(size_t offset, size_t size, unsigned int location)
{
	
}

inline void VertexBufferDX12::unbind()
{
	
}

inline size_t VertexBufferDX12::getSize() {
	return totalSize;
}

ID3D12Resource1** VertexBufferDX12::getVertexBufferResource()
{
	return &vertexBufferResource;
}

D3D12_VERTEX_BUFFER_VIEW* VertexBufferDX12::getVertexBufferView()
{
	return &vertexBufferView;
}
