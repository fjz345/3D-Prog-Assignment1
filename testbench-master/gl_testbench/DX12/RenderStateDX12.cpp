#include <GL/glew.h>
#include "RenderStateDX12.h"

RenderStateDX12::RenderStateDX12()
{
	_wireframe = false;
}

RenderStateDX12::~RenderStateDX12()
{
}

void RenderStateDX12::set()
{	
}

/*
	Keep a pointer to the global wireframe state
*/
void RenderStateDX12::setGlobalWireFrame(bool* global)
{
	this->globalWireFrame = global;
}

bool RenderStateDX12::IsWireframe()
{
	return _wireframe;
}

ID3D12PipelineState** RenderStateDX12::GetPSO()
{
	return &PSO;
}

/*
 set wireframe mode for this Render state
*/
void RenderStateDX12::setWireFrame(bool wireframe) {
	_wireframe = wireframe;
}
