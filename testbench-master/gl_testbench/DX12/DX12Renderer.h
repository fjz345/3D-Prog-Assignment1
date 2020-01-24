#pragma once

#include "../Renderer.h"

#include <SDL.h>
#include <GL/glew.h>
#include "stdafx.h"
#include <SDL_Syswm.h>

#define NUM_SWAP_BUFFERS 2

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")



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
	void CreateSDLWindow(unsigned int width = 640, unsigned int height = 480);
	void CreateDXDevice();
	void CreateCommandInterface();
	void CreateSwapChain();
	void CreateFenceAndEventHandle();
	void CreateRenderTarget();
	void CreateViewport();
	void CreateScissorRect();

	SDL_Window* window;
	HWND wndHandle;
	ID3D12Device5* device5 = nullptr;

	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList3* commandList3 = nullptr;

	// TODO: Is this same as 4?
	IDXGISwapChain3* swapChain3 = nullptr;



	
	bool globalWireframeMode = false;
};

