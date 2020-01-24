#include "VertexBufferDX12.h"
#include "MeshDX12.h"
#include <assert.h>

/*
	Create a VertexBuffer backed in gpu mem
	- todo: add error checking...
*/
VertexBufferDX12::VertexBufferDX12(size_t size, DATA_USAGE usage) {
	
}

VertexBufferDX12::~VertexBufferDX12()
{
	
}

/*
	SSBO based Vertex Buffer
*/
void VertexBufferDX12::setData(const void* data, size_t size,  size_t offset)
{
	
}

/*
 bind at "location", with offset "offset", "size" bytes 
 */
void VertexBufferDX12::bind(size_t offset, size_t size, unsigned int location) {
	
}

inline void VertexBufferDX12::unbind() {
	
}

inline size_t VertexBufferDX12::getSize() {
	return totalSize;
}
