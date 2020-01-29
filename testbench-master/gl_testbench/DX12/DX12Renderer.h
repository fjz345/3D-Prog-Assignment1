#pragma once

#include "../Renderer.h"

#include <SDL.h>
#include <GL/glew.h>
#include "stdafx.h"
#include <SDL_Syswm.h>

#define NUM_SWAP_BUFFERS 2

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

enum
{
	ROOT_SIGNATURE_POS,
	ROOT_SIGNATURE_NOR,
	ROOT_SIGNATURE_UV,
	ROOT_SIGNATURE_COLOUR,
	ROOT_SIGNATURE_TRANS
};


class DX12Renderer : public Renderer
{
public:
	DX12Renderer();
	~DX12Renderer();

	Material* makeMaterial(const std::string& name);
	Mesh* makeMesh();
	//VertexBuffer* makeVertexBuffer();
	VertexBuffer* makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage);
	ConstantBuffer* makeConstantBuffer(std::string NAME, unsigned int location);
//	ResourceBinding* makeResourceBinding();
	RenderState* makeRenderState();
	Technique* makeTechnique(Material* m, RenderState* r);
	Texture2D* makeTexture2D();
	Sampler2D* makeSampler2D();
	std::string getShaderPath();
	std::string getShaderExtension();

	int initialize(unsigned int width = 640, unsigned int height = 480);
	void setWinTitle(const char* title);
	int shutdown();

	void setClearColor(float, float, float, float);
	void clearBuffer(unsigned int);
//	void setRenderTarget(RenderTarget* rt); // complete parameters
	void setRenderState(RenderState* ps);
	void submit(Mesh* mesh);
	void frame();
	void present();

private:
	void WaitForGpu();

	void CreateSDLWindow(unsigned int width = 640, unsigned int height = 480);
	void CreateDXDevice();
	void CreateCommandInterface();
	void CreateSwapChain();
	void CreateFenceAndEventHandle();
	void CreateRenderTarget();
	void CreateViewport(unsigned int width, unsigned int height);
	void CreateScissorRect(unsigned int width, unsigned int height);
	void CreateRootSignature();

	SDL_Window* window;
	HWND wndHandle;
	ID3D12Device5* device5 = nullptr;

	// Command Interfaces
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList3* commandList3 = nullptr;

	// TODO: Is this same as 4?
	IDXGISwapChain3* swapChain3 = nullptr;

	// Fences
	ID3D12Fence1* fence = nullptr;
	HANDLE eventHandle = nullptr;
	UINT64 fenceValue = 0;

	// RenderTarget
	ID3D12DescriptorHeap* renderTargetsHeap = nullptr;
	ID3D12Resource1* renderTargets[NUM_SWAP_BUFFERS] = {};
	UINT renderTargetDescriptorSize = 0;
	int currBackBuffer = 0;

	// Viewport & ScissorRect
	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};

	// Root Discriptor
	ID3D12RootSignature* rootSig;


	int numVertexBuffers = 0;
	
	bool globalWireframeMode = false;

	std::unordered_map<Technique*, std::vector<Mesh*>> drawList2;
};

// TODO: Allmänt, nästa steg... Rootsignatures & PSO's, ska man ha 1 RS per pipeline?