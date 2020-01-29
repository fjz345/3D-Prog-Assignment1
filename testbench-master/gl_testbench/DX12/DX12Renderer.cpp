#include <stdio.h>
#include "DX12Renderer.h"
#include <GL/glew.h>

#include "MaterialDX12.h"
#include "MeshDX12.h"
#include "../Technique.h"
#include "ResourceBindingDX12.h"
#include "RenderStateDX12.h"
#include "VertexBufferDX12.h"
#include "ConstantBufferDX12.h"
#include "Texture2DDX12.h"

DX12Renderer::DX12Renderer()
{
}

DX12Renderer::~DX12Renderer()
{
}

int DX12Renderer::shutdown()
{
	SDL_Quit();
	return 0;
}

Mesh* DX12Renderer::makeMesh() {
	return new MeshDX12(); 
}

Texture2D* DX12Renderer::makeTexture2D()
{
	return (Texture2D*)new Texture2DDX12();
}

Sampler2D* DX12Renderer::makeSampler2D()
{
	return (Sampler2D*)new Sampler2DDX12();
}

ConstantBuffer* DX12Renderer::makeConstantBuffer(std::string NAME, unsigned int location) {
	return new ConstantBufferDX12(NAME, location);
}

std::string DX12Renderer::getShaderPath() {
	return std::string("..\\assets\\HLSL\\");
}

std::string DX12Renderer::getShaderExtension() {
	return std::string(".hlsl");
}

VertexBuffer* DX12Renderer::makeVertexBuffer( size_t size, VertexBuffer::DATA_USAGE usage) {

	D3D12_HEAP_PROPERTIES hp = {};
	hp.Type = D3D12_HEAP_TYPE_UPLOAD;
	hp.CreationNodeMask = 1;
	hp.VisibleNodeMask = 1;

	// TODO: Stefan, ifall man skall dynamiskt öka size, hur gör man då?
	D3D12_RESOURCE_DESC rd = {};
	rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rd.Width = size;
	rd.Height = 1;
	rd.DepthOrArraySize = 1;
	rd.MipLevels = 1;
	rd.SampleDesc.Count = 1;
	rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;




	VertexBufferDX12* VB = new VertexBufferDX12(size, usage);

	ID3D12Resource1** VBResourcePointer = VB->getVertexBufferResource();

	// TODO: Skall vi inte använda denna funktionen?
	device5->CreateCommittedResource(
		&hp,
		D3D12_HEAP_FLAG_NONE,
		&rd,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(VBResourcePointer));




	/*
	D3D12_DESCRIPTOR_HEAP_DESC dhd = {};
	dhd.NumDescriptors = 4;
	dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	ID3D12DescriptorHeap* dtHeap;

	auto hr = device5->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&dtHeap));

	UINT dtDescriptorSize = device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// OBS(Finns en GetGPU....) funktion
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = dtHeap->GetCPUDescriptorHandleForHeapStart();

	// One RTV for each frame
	for (UINT n = 0; n < 4; n++)
	{
		// Behöver en ID3D12Heap*??????
		device5->CreatePlacedResource(, )
		cdh.ptr += renderTargetDescriptorSize;
	}
	*/

	
	// TODO: HOW TO DO THIS
	std::string temp = "vb heap" + std::to_string(numVertexBuffers);
	numVertexBuffers++;
	std::wstring wtemp = std::wstring(temp.begin(), temp.end());
	LPCWSTR name = wtemp.c_str();

	(*VBResourcePointer)->SetName(name);

	// Initialize vertexbufferview, used in render call
	D3D12_VERTEX_BUFFER_VIEW* VBView = VB->getVertexBufferView();

	int numberOfTriangles = 100;

	VBView->BufferLocation = (*VBResourcePointer)->GetGPUVirtualAddress();
	VBView->StrideInBytes = numberOfTriangles / 100; // Hårdkodat, räkna ut stride:n på pos/norm/uv ---> 3*sizeof(float)/3*sizeof(float)/2*sizeof(float)
	VBView->SizeInBytes = size;


	return VB;
};

Material* DX12Renderer::makeMaterial(const std::string& name) {
	return new MaterialDX12(name);
}

Technique* DX12Renderer::makeTechnique(Material* m, RenderState* r) {
	

	MaterialDX12 * mat = reinterpret_cast<MaterialDX12*>(m);
	// PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};

	ID3DBlob* a = mat->shaderBlobs[int(Material::ShaderType::VS)];

	gpsd.pRootSignature = rootSig;
	gpsd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsd.VS.pShaderBytecode = reinterpret_cast<void*>(mat->shaderBlobs[int(Material::ShaderType::VS)]->GetBufferPointer());
	gpsd.VS.BytecodeLength = mat->shaderBlobs[int(Material::ShaderType::VS)]->GetBufferSize();
	gpsd.PS.pShaderBytecode = reinterpret_cast<void*>(mat->shaderBlobs[int(Material::ShaderType::PS)]->GetBufferPointer());
	gpsd.PS.BytecodeLength = mat->shaderBlobs[int(Material::ShaderType::PS)]->GetBufferSize();

	//Specify render target and depthstencil usage.
	gpsd.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsd.NumRenderTargets = 1;

	gpsd.SampleDesc.Count = 1;
	gpsd.SampleMask = UINT_MAX;

	//Specify rasterizer behaviour.
	RenderStateDX12* rDX12 = reinterpret_cast<RenderStateDX12*>(r);
	if (rDX12->IsWireframe())
	{
		gpsd.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		gpsd.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}
		
	else
	{
		gpsd.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		gpsd.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	}

	ID3D12PipelineState** PSO = rDX12->GetPSO();
	device5->CreateGraphicsPipelineState(&gpsd,IID_PPV_ARGS(PSO)); // Varför fungerar inte IID_PPV_ARGS(&rDX12->GetPSO())

	Technique* t = new Technique(m, r);
	return t;
}

RenderState* DX12Renderer::makeRenderState() {
	RenderStateDX12* newRS = new RenderStateDX12();
	newRS->setGlobalWireFrame(&this->globalWireframeMode);
	newRS->setWireFrame(false);
	return (RenderState*)newRS;
}

void DX12Renderer::setWinTitle(const char* title) {
	SDL_SetWindowTitle(this->window, title);
}

int DX12Renderer::initialize(unsigned int width, unsigned int height) {

	CreateSDLWindow(width, height);

	CreateDXDevice();

	CreateCommandInterface();

	CreateSwapChain();

	CreateFenceAndEventHandle();

	CreateRenderTarget();

	CreateViewport(width, height);
	
	CreateScissorRect(width, height);

	CreateRootSignature();

	return 0;
}

/*
 Super simplified implementation of a work submission
 TODO.
*/


void DX12Renderer::submit(Mesh* mesh)
{
	
};

/*
 Naive implementation, no re-ordering, checking for state changes, etc.
 TODO.
*/
void DX12Renderer::frame()
{
	
};

void DX12Renderer::present()
{
	SDL_GL_SwapWindow(window);
}

void DX12Renderer::WaitForGpu()
{
	//WAITING FOR EACH FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	//This is code implemented as such for simplicity. The cpu could for example be used
	//for other tasks to prepare the next frame while the current one is being rendered.

	//Signal and increment the fence value.
	const UINT64 oldFence = fenceValue;
	commandQueue->Signal(fence, oldFence);
	fenceValue++;

	//Wait until command queue is done.
	if (fence->GetCompletedValue() < oldFence)
	{
		fence->SetEventOnCompletion(oldFence, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
	}
}

void DX12Renderer::CreateSDLWindow(unsigned int width, unsigned int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "%s", SDL_GetError());
		exit(-1);
	}

	window = SDL_CreateWindow("DirectX12", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version); /* initialize info structure with SDL version info */

	// Save the wndHandle
	SDL_GetWindowWMInfo(window, &info);
	wndHandle = info.info.win.window;
}

void DX12Renderer::CreateDXDevice()
{
	IDXGIFactory4* factory = nullptr;
	IDXGIAdapter1* adapter = nullptr;

	CreateDXGIFactory(IID_PPV_ARGS(&factory));

	for (UINT adapterIndex = 0;; ++adapterIndex)
	{
		adapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &adapter))
		{
			break; // No more adapters
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device5), nullptr)))
		{
			break;
		}

		SafeRelease(&adapter);
	}

	if (adapter)
	{
		HRESULT hr = S_OK;
		//Create the actual device.
		if (SUCCEEDED(hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device5))))
		{

		}

		SafeRelease(&adapter);
	}
	else
	{
		//Create warp device if no adapter was found.
		factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter));
		D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device5));
	}

	SafeRelease(&factory);
}
void DX12Renderer::CreateCommandInterface()
{
	D3D12_COMMAND_QUEUE_DESC cqd = {};
	device5->CreateCommandQueue(&cqd, IID_PPV_ARGS(&commandQueue));

	device5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));

	device5->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator,
		nullptr,
		IID_PPV_ARGS(&commandList3));

	commandList3->Close();
}

void DX12Renderer::CreateSwapChain()
{
	IDXGIFactory4* factory = nullptr;
	CreateDXGIFactory(IID_PPV_ARGS(&factory));

	//Create descriptor
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = 0;
	scDesc.Height = 0;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.Stereo = FALSE;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = NUM_SWAP_BUFFERS;
	scDesc.Scaling = DXGI_SCALING_NONE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.Flags = 0;
	scDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	IDXGISwapChain1* swapChain1 = nullptr;
	if (SUCCEEDED(factory->CreateSwapChainForHwnd(
		commandQueue,
		wndHandle,
		&scDesc,
		nullptr,
		nullptr,
		&swapChain1)))
	{
		if (SUCCEEDED(swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain3))))
		{
			swapChain3->Release();
		}
	}

	SafeRelease(&factory);
}

void DX12Renderer::CreateFenceAndEventHandle()
{
	device5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	fenceValue = 1;

	// Event handle to use for GPU synchronization
	eventHandle = CreateEvent(0, false, false, 0);
}

void DX12Renderer::CreateRenderTarget()
{
	// Fill out descriptor for the render target views
	D3D12_DESCRIPTOR_HEAP_DESC dhd = {};
	dhd.NumDescriptors = NUM_SWAP_BUFFERS;
	dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	auto hr = device5->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&renderTargetsHeap));

	renderTargetDescriptorSize = device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// OBS(Finns en GetGPU....) funktion
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetsHeap->GetCPUDescriptorHandleForHeapStart();

	// One RTV for each frame
	for (UINT n = 0; n < NUM_SWAP_BUFFERS; n++)
	{
		// S�tter en pekare till v�ran rendertarget fr�n swapchain. S� att swapchain vet vilken RTV den har.
		hr = swapChain3->GetBuffer(n, IID_PPV_ARGS(&renderTargets[n]));
		device5->CreateRenderTargetView(renderTargets[n], nullptr, cdh);
		cdh.ptr += renderTargetDescriptorSize;
	}
	
}

void DX12Renderer::CreateViewport(unsigned int width, unsigned int height)
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void DX12Renderer::CreateScissorRect(unsigned int width, unsigned int height)
{
	scissorRect.left = (long)0;
	scissorRect.right = (long)width;
	scissorRect.top = (long)0;
	scissorRect.bottom = (long)height;
}

void DX12Renderer::CreateRootSignature()
{
	// TODO: Vad exakt är en SRV, varför använder vi inte CBV:s
	D3D12_DESCRIPTOR_RANGE dtRanges[1];
	dtRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	dtRanges[0].NumDescriptors = 4; // 4 SRV:s
	dtRanges[0].BaseShaderRegister = 0; //register t0
	dtRanges[0].RegisterSpace = 0; //register(b0,space0);
	dtRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Create Descriptor table with range above
	D3D12_ROOT_DESCRIPTOR_TABLE dt;
	dt.NumDescriptorRanges = ARRAYSIZE(dtRanges);
	dt.pDescriptorRanges = dtRanges;

	// 2 RootParams, 1 dt->(4 SRV), 1 CBV
	D3D12_ROOT_PARAMETER rootParam[2];
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[0].DescriptorTable = dt;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // TODO: Fixa rätt shaderVisibility

	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[1].Descriptor.ShaderRegister = 0;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rsDesc;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;	// We dont use input layout... 
	rsDesc.NumParameters = ARRAYSIZE(rootParam);
	rsDesc.pParameters = rootParam;
	rsDesc.NumStaticSamplers = 0; // TODO: Create Static Sampler
	rsDesc.pStaticSamplers = nullptr;	// Vad händer här?

	ID3DBlob* sBlob;

	auto result = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&sBlob,
		nullptr);

	device5->CreateRootSignature(
		0,
		sBlob->GetBufferPointer(),
		sBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSig));

}

void DX12Renderer::setClearColor(float r, float g, float b, float a)
{
	
}

void DX12Renderer::clearBuffer(unsigned int flag)
{
	
}

//void OpenGLRenderer::setRenderTarget(RenderTarget* rt) {};

void DX12Renderer::setRenderState(RenderState* ps)
{
	// naive implementation
	ps->set();
}
