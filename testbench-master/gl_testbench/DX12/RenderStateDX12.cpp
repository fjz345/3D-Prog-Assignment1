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
	// was wireframe mode already set?
	if (*globalWireFrame == _wireframe)
		return;
	else
		*globalWireFrame = _wireframe;

	if (_wireframe)
	{

	}
	else
	{

	}
		
}

/*
	Keep a pointer to the global wireframe state
*/
void RenderStateDX12::setGlobalWireFrame(bool* global)
{
	this->globalWireFrame = global;
}

/*
 set wireframe mode for this Render state
*/
void RenderStateDX12::setWireFrame(bool wireframe) {
	_wireframe = wireframe;
}
