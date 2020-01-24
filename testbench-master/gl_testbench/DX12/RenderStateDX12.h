#pragma once
#include <vector>
#include "../RenderState.h"

class RenderStateDX12 : public RenderState
{
public:
	RenderStateDX12();
	~RenderStateDX12();
	void setWireFrame(bool);
	void set();

	void setGlobalWireFrame(bool* global);
private:
	bool _wireframe;
	bool* globalWireFrame;
};

