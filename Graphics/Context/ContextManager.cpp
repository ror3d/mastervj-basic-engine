#include "ContextManager.h"

#include "VertexTypes.h"
#include "Renderable/RenderableVertexs.h"
#include "Effect/Effect.h"

#include <Math/Matrix44.h>

//#include "Debug/DebugRender.h"
#include <Base/Math/Color.h>


#pragma comment(lib,"d3d11.lib")


CContextManager::CContextManager()
	: m_D3DDevice(nullptr)
	, m_DeviceContext(nullptr)
	, m_SwapChain(nullptr)
	, m_RenderTargetView(nullptr)
	, m_DepthStencil(nullptr)
	, m_DepthStencilView(nullptr)
	, m_D3DDebug(nullptr)
{

}


CContextManager::~CContextManager()
{
}

void CContextManager::destroy()
{
	CHECKED_RELEASE(m_RenderTargetView);
	CHECKED_RELEASE(m_DepthStencil);
	CHECKED_RELEASE(m_DepthStencilView);

	if (m_SwapChain)
	{
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_SwapChain->SetFullscreenState(FALSE, nullptr);
	}

	if (m_D3DDebug)
		m_D3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	CHECKED_RELEASE(m_D3DDebug);

	CHECKED_RELEASE(m_D3DDevice);
	CHECKED_RELEASE(m_DeviceContext);
	CHECKED_RELEASE(m_SwapChain);
}


HRESULT CContextManager::CreateContext(HWND hWnd, int Width, int Height)
{

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

#if _DEBUG
	int flags = D3D11_CREATE_DEVICE_DEBUG;
#else
	int flags = 0;
#endif

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_D3DDevice, NULL, &m_DeviceContext)))
	{
		return S_FALSE;
	}

#if _DEBUG
	HRESULT hr = m_D3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_D3DDebug));
	if (FAILED(hr))
		return hr;
#endif


	// treure el ALT+INTRO autom�tic
	IDXGIFactory* dxgiFactory;
	hr = m_SwapChain->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
	assert(hr == S_OK);

	hr = dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	assert(hr == S_OK);

	dxgiFactory->Release();

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Width;
	vp.Height = (FLOAT)Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);

	return S_OK;
}

HRESULT CContextManager::CreateBackBuffer(HWND hWnd, int Width, int Height)
{

	CHECKED_RELEASE(m_RenderTargetView);
	CHECKED_RELEASE(m_DepthStencil);
	CHECKED_RELEASE(m_DepthStencilView);

	m_Width = Width;
	m_Height = Height;

	ID3D11Texture2D *pBackBuffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return FALSE;


	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_D3DDevice->CreateTexture2D(&descDepth, NULL, &m_DepthStencil);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_D3DDevice->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView);
	if (FAILED(hr))
		return hr;

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	return S_OK;
}



void CContextManager::Resize(HWND hWnd, unsigned int Width, unsigned int Height)
{
	if (m_D3DDevice != nullptr)
	{
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		CHECKED_RELEASE(m_RenderTargetView);
		CHECKED_RELEASE(m_DepthStencil);
		CHECKED_RELEASE(m_DepthStencilView);

		m_SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
		HRESULT hr = CreateBackBuffer(hWnd, Width, Height);
		assert(hr == S_OK);
	}
}

void CContextManager::BeginRender()
{
	BeginRender(CColor(0, 0, 0));
}

void CContextManager::BeginRender(const CColor &backgroundColor)
{
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_Width;
	vp.Height = (FLOAT)m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, &backgroundColor.x);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void CContextManager::EndRender()
{
	m_SwapChain->Present(0, 0);
}

void CContextManager::EnableAlphaBlendState()
{
	D3D11_BLEND_DESC l_AlphablendDesc;
	ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
	l_AlphablendDesc.RenderTarget[0].BlendEnable = true;
	l_AlphablendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	l_AlphablendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	l_AlphablendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	/*CreateBlendState not exist*/
	//HRESULT hr = m_DeviceContext->CreateBlendState(&l_AlphablendDesc, &m_AlphaBlendState);
	//assert(hr);

	m_DeviceContext->OMSetBlendState(m_AlphaBlendState, NULL, 0xffffffff);
}

void CContextManager::DisableAlphaBlendState()
{
	m_DeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void CContextManager::Clear(bool clear_DepthStencil, bool clear_RenderTarget){
	
	if (clear_DepthStencil){
		CColor color = CColor(0, 0, 0);
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, &color.x);		
	}

	if (clear_RenderTarget)		
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CContextManager::SetRenderTargets(int NumViews, ID3D11RenderTargetView *const	*RenderTargetViews, ID3D11DepthStencilView *DepthStencilView){
	m_NumViews = NumViews;
	m_CurrentRenderTargetViews = *RenderTargetViews;
	m_CurrentDepthStencilView = DepthStencilView;
	m_DeviceContext->OMSetRenderTargets(m_NumViews, RenderTargetViews,	DepthStencilView);
}

void CContextManager::UnsetRenderTargets(){
	m_NumViews = 1;
	m_CurrentRenderTargetViews = m_RenderTargetView;
	m_CurrentDepthStencilView = m_DepthStencilView;
	m_DeviceContext->OMSetRenderTargets(m_NumViews, &m_RenderTargetView, m_DepthStencilView);
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_Width;
	vp.Height = (FLOAT)m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);
}

void CContextManager::DrawScreenQuad(CEffectTechnique *EffectTechnique, CTexture
	*Texture, float x, float y, float Width, float Height, const CColor &Color)
{
	//CEffectManager::m_SceneParameters.m_BaseColor = Color;
	if (Texture != NULL)
		Texture->Activate(0);
	D3D11_VIEWPORT l_Viewport;
	//l_Viewport.Width = Width*m_Viewport.Width;
	//l_Viewport.Height = Height*m_Viewport.Height;
	l_Viewport.MinDepth = 0.0f;
	l_Viewport.MaxDepth = 1.0f;
	//l_Viewport.TopLeftX = x*m_Viewport.Width;
	//l_Viewport.TopLeftY = y*m_Viewport.Height;
	//m_DeviceContext->RSSetViewports(1, &l_Viewport);
	//m_DrawQuadRV->Render(this, EffectTechnique,	&CEffectManager::m_SceneParameters);
	//m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

void CContextManager::Present(){
	m_SwapChain->Present(0, 0);
}