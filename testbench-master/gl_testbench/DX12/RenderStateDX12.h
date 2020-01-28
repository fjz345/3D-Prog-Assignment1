#pragma once
#include <vector>
#include "../RenderState.h"
#include <stdafx.h>

class RenderStateDX12 : public RenderState
{
public:
	RenderStateDX12();
	~RenderStateDX12();
	void setWireFrame(bool);
	void set();

	void setGlobalWireFrame(bool* global);

	bool IsWireframe();

	ID3D12PipelineState** GetPSO();

private:
	bool _wireframe;
	bool* globalWireFrame;

	ID3D12PipelineState* PSO;
};

