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
	Texture2DDX12* texture = new Texture2DDX12(this);

	return (Texture2D*)texture;
}

Sampler2D* DX12Renderer::makeSampler2D()
{
	return (Sampler2D*)new Sampler2DDX12();
}

ConstantBuffer* DX12Renderer::makeConstantBuffer(std::string NAME, unsigned int location) 
{	
	// TODO: Vad är detta
	UINT cbSizeAligned = (sizeof(ConstantBuffer) + 255) & ~255;	// 256-byte aligned CB.

	// TODO: Skapar heap properties efter vi har skapat heapen???????????????????
	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = 1; //used when multi-gpu
	heapProperties.VisibleNodeMask = 1; //used when multi-gpu
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(float) * 4;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ConstantBuffer* CB = new ConstantBufferDX12(NAME, location);
	ConstantBufferDX12* CBDX12 = reinterpret_cast<ConstantBufferDX12*>(CB);

	ID3D12Resource1** constantBufferResource = CBDX12->getConstantBufferResource();

	device5->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constantBufferResource)
	);
	(*constantBufferResource)->SetName(L"cb heap");

	return CB;
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
	rd.Width = size / 100; // Vi gör endast 1 triangel (inte 100 som han vill ha i sin kod)
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
	
	// TODO: HOW TO DO THIS
	std::string temp = "vb heap" + std::to_string(numVertexBuffers);
	numVertexBuffers++;
	std::wstring wtemp = std::wstring(temp.begin(), temp.end());
	LPCWSTR name = wtemp.c_str();

	(*VBResourcePointer)->SetName(name);

	return VB;
};

Material* DX12Renderer::makeMaterial(const std::string& name) {

	MaterialDX12 * mat = new MaterialDX12(name);

	int location = 6;
	mat->constantBuffers[location] = new ConstantBufferDX12(name, location);

	// TODO: Vad är detta
	UINT cbSizeAligned = (sizeof(ConstantBuffer) + 255) & ~255;	// 256-byte aligned CB.

	// TODO: Skapar heap properties efter vi har skapat heapen???????????????????
	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.CreationNodeMask = 1; //used when multi-gpu
	heapProperties.VisibleNodeMask = 1; //used when multi-gpu
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(float) * 4;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ConstantBufferDX12* CBDX12 = reinterpret_cast<ConstantBufferDX12*>(mat->constantBuffers[location]);

	ID3D12Resource1** constantBufferResource = CBDX12->getConstantBufferResource();

	device5->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constantBufferResource)
	);
	(*constantBufferResource)->SetName(L"cb heap");

	return mat;
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
		gpsd.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}

	//Specify blend descriptions.	TODO:: HJÄLP
	D3D12_RENDER_TARGET_BLEND_DESC defaultRTdesc = {
		false, false,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL };
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		gpsd.BlendState.RenderTarget[i] = defaultRTdesc;


	// Depth descriptor
	D3D12_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// Om pixels depth är lägre än den gamla så ritas den nya ut

	// TODO: Vad är stencil?
	dsd.StencilEnable = false;
	dsd.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	const D3D12_DEPTH_STENCILOP_DESC defaultStencilOP{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
	dsd.FrontFace = defaultStencilOP;
	dsd.BackFace = defaultStencilOP;

	gpsd.DepthStencilState = dsd;

	ID3D12PipelineState** PSO = rDX12->GetPSO();
	auto hr = device5->CreateGraphicsPipelineState(&gpsd,IID_PPV_ARGS(PSO)); // Varför fungerar inte IID_PPV_ARGS(&rDX12->GetPSO())

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

	CreateDepthStencil();

	CreateViewport(width, height);
	
	CreateScissorRect(width, height);

	CreateRootSignature();

	CreateDescriptorHeap();

	return 0;
}

/*
 Super simplified implementation of a work submission
 TODO.
*/


void DX12Renderer::submit(Mesh* mesh)
{
	drawList2[mesh->technique].push_back(mesh);
};

/*
 Naive implementation, no re-ordering, checking for state changes, etc.
 TODO.
*/
void DX12Renderer::frame()
{
	currBackBuffer = swapChain3->GetCurrentBackBufferIndex();

	// Get the first mesh to get the triangle data for rootSig
	Mesh* mesh = drawList2.begin()->second[0];

	auto GeometryMap = mesh->geometryBuffers;
	VertexBufferDX12* VBPos = reinterpret_cast<VertexBufferDX12*>(GeometryMap[0].buffer);
	VertexBufferDX12* VBNor = reinterpret_cast<VertexBufferDX12*>(GeometryMap[1].buffer);
	VertexBufferDX12* VBuv = reinterpret_cast<VertexBufferDX12*>(GeometryMap[2].buffer);

	// Set root signature
	commandList3->SetGraphicsRootSignature(rootSig);

	// Set root descriptor table TODO: hjälp, förståelse
	//commandList3->SetGraphicsRootDescriptorTable(RS_TEXTURE,
	//	descriptorHeap->GetGPUDescriptorHandleForHeapStart());
	
	// Sätter 1 triangel data.
	commandList3->SetGraphicsRootShaderResourceView(RS_POSITION,
		(*VBPos->getVertexBufferResource())->GetGPUVirtualAddress());

	commandList3->SetGraphicsRootShaderResourceView(RS_NORMAL,
		(*VBNor->getVertexBufferResource())->GetGPUVirtualAddress());

	commandList3->SetGraphicsRootShaderResourceView(RS_UV,
		(*VBuv->getVertexBufferResource())->GetGPUVirtualAddress());

	// Ändra state på front/backbuffer
	commandList3->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		renderTargets[currBackBuffer],
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	// TODO: varför CPU? fattar ej helt förra gången
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetsHeap->GetCPUDescriptorHandleForHeapStart();
	cdh.ptr += renderTargetDescriptorSize * currBackBuffer;
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvhandle = depthStencilHeap->GetCPUDescriptorHandleForHeapStart();
	//dsvhandle.ptr += depthStencilDescriptorSize * currBackBuffer;
	commandList3->OMSetRenderTargets(1, &cdh, true, nullptr);

	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	commandList3->ClearRenderTargetView(cdh, clearColor, 0, nullptr);

	// Clear Depthbuffer	TODO: Funkar ej
	//commandList3->ClearDepthStencilView(dsvhandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	commandList3->RSSetViewports(1, &viewport);
	commandList3->RSSetScissorRects(1, &scissorRect);
	commandList3->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Resetta depthbuffer

	// Körs 4 gånger
	for (auto work : drawList2)
	{
		// Kanske vänta på GPU...? Se sista raden innan nästa iteration med eexecuteCommandList


		// Här inne körs SetPipelineState (first är technique)
		work.first->enable(this);

		// Bind color
		MaterialDX12* MatDX12 = reinterpret_cast<MaterialDX12*>(work.first->getMaterial());
		ConstantBufferDX12* CBCDX12 = reinterpret_cast<ConstantBufferDX12*>(MatDX12->constantBuffers[6]);

		commandList3->SetGraphicsRootConstantBufferView(RS_COLOR, (*CBCDX12->getConstantBufferResource())->GetGPUVirtualAddress());
		

		// För varje technique som hör ihop med sina meshar
		for (auto mesh : work.second)
		{
			size_t numberElements = mesh->geometryBuffers[0].numElements;
			

			// Bind translation CBV
			auto CBTDX12 = reinterpret_cast<ConstantBufferDX12*>(mesh->txBuffer);
			commandList3->SetGraphicsRootConstantBufferView(RS_TRANSLATION, (*CBTDX12->getConstantBufferResource())->GetGPUVirtualAddress());

			// Binda texturer här sen

			// Binda vertexbuffers (vet ej hur vi gör detta nu när vi inte har input layout)
			for (auto element : mesh->geometryBuffers) 
			{
				
			}
			// Fixa constantbuffers...

			// Tror Draw ska ske här

			commandList3->DrawInstanced(3, 1, 0, 0);
		}

		
	}

	// Ändra state på front/backbuffer
	commandList3->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		renderTargets[currBackBuffer],
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	commandList3->Close();

	ID3D12CommandList* listsToExecute[] = { commandList3};
	commandQueue->ExecuteCommandLists(ARRAYSIZE(listsToExecute), listsToExecute);

	drawList2.clear();

	WaitForGpu();
};

void DX12Renderer::present()
{
	swapChain3->Present(0, 0);
	//SDL_GL_SwapWindow(window);
}

ID3D12Device5* DX12Renderer::Getdevice5()
{
	return this->device5;
}

ID3D12CommandQueue* DX12Renderer::GetCommandQueue()
{
	return this->commandQueue;
}

ID3D12GraphicsCommandList3* DX12Renderer::GetCommandList()
{
	return this->commandList3;
}

ID3D12DescriptorHeap* DX12Renderer::GetDH()
{
	return descriptorHeap;
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
#ifdef _DEBUG
	//Enable the D3D12 debug layer.
	ID3D12Debug* debugController = nullptr;

#ifdef STATIC_LINK_DEBUGSTUFF
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	SafeRelease(debugController);
#else
	HMODULE mD3D12 = GetModuleHandle(L"D3D12.dll");
	PFN_D3D12_GET_DEBUG_INTERFACE f = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(mD3D12, "D3D12GetDebugInterface");
	if (SUCCEEDED(f(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	SafeRelease(&debugController);
#endif
#endif

	IDXGIFactory6* factory = nullptr;
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
		// TODO: Exception thrown at ___________
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

void DX12Renderer::CreateDepthStencil()
{
	// Fill out descriptor for the depthStencil views
	D3D12_DESCRIPTOR_HEAP_DESC dhd = {};
	dhd.NumDescriptors = NUM_SWAP_BUFFERS;
	dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	auto hr = device5->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&depthStencilHeap));

	depthStencilDescriptorSize = device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = depthStencilHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;
	
	// One DSV for each frame
	for (UINT n = 0; n < NUM_SWAP_BUFFERS; n++)
	{
		device5->CreateDepthStencilView(depthStencilBuffers[n], &depthStencilDesc, cdh);
		cdh.ptr += depthStencilDescriptorSize;
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
	dtRanges[0].NumDescriptors = 1; // 1 SRV (Texture)
	dtRanges[0].BaseShaderRegister = 0; //register t0
	dtRanges[0].RegisterSpace = 0; //register(b0,space0);
	dtRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Create Descriptor table with range above
	D3D12_ROOT_DESCRIPTOR_TABLE dt;
	dt.NumDescriptorRanges = ARRAYSIZE(dtRanges);
	dt.pDescriptorRanges = dtRanges;

	// 2 RootParams, 1 dt->(4 SRV), 2 CBV
	D3D12_ROOT_PARAMETER rootParam[6]{};	// register space var ej satt!, nu nollad
	rootParam[RS_TEXTURE].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[RS_TEXTURE].DescriptorTable = dt;
	rootParam[RS_TEXTURE].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // TODO: Fixa rätt shaderVisibility

	rootParam[RS_POSITION].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParam[RS_POSITION].Descriptor.ShaderRegister = 1;
	rootParam[RS_POSITION].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS_NORMAL].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParam[RS_NORMAL].Descriptor.ShaderRegister = 2;
	rootParam[RS_NORMAL].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS_UV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParam[RS_UV].Descriptor.ShaderRegister = 3;
	rootParam[RS_UV].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS_COLOR].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[RS_COLOR].Descriptor.ShaderRegister = 0;
	rootParam[RS_COLOR].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS_TRANSLATION].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[RS_TRANSLATION].Descriptor.ShaderRegister = 1;
	rootParam[RS_TRANSLATION].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

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

void DX12Renderer::CreateDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDescriptorDesc = {};
	heapDescriptorDesc.NumDescriptors = 1; // Endast 1 textur i programmet
	heapDescriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	device5->CreateDescriptorHeap(&heapDescriptorDesc, IID_PPV_ARGS(&descriptorHeap));
}

void DX12Renderer::setClearColor(float r, float g, float b, float a)
{
	
}

void DX12Renderer::clearBuffer(unsigned int flag)
{
	commandAllocator->Reset();

	commandList3->Reset(commandAllocator, NULL);
}

//void OpenGLRenderer::setRenderTarget(RenderTarget* rt) {};

void DX12Renderer::setRenderState(RenderState* ps)
{
	// naive implementation
	//ps->set();

	RenderStateDX12 *PSODX12 = reinterpret_cast<RenderStateDX12*>(ps);

	commandList3->SetPipelineState(*PSODX12->GetPSO());
}
