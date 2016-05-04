#include "DynamicTexture.h"
#include "Engine/Engine.h"
#include <Graphics/Context/ContextManager.h>
#include <map>

CDynamicTexture::CDynamicTexture(const std::string &Name, int Width, int Height, bool CreateDepthStencilBuffer, DXGI_FORMAT format)
	: CTexture(Name)
	, m_Width(Width)
	, m_Height(Height)
	, m_CreateDepthStencilBuffer(CreateDepthStencilBuffer)
	, m_DepthStencilBuffer(nullptr)
	, m_DepthStencilView(nullptr)
	, m_RenderTargetView(nullptr)
	, m_RenderTargetTexture(nullptr)
{
	m_format = format;
	Init();
}

CDynamicTexture::CDynamicTexture(const CXMLTreeNode &node)
	: CTexture(node.GetPszProperty("name"))
	, m_DepthStencilBuffer(nullptr)
	, m_DepthStencilView(nullptr)
	, m_RenderTargetView(nullptr)
	, m_RenderTargetTexture(nullptr)
{
	int width = 64;
	int height = 64;
	if (node.GetBoolProperty("texture_width_as_frame_buffer", false, false))
	{
		width = CEngine::GetSingleton().getContextManager()->GetWidth();
		height = CEngine::GetSingleton().getContextManager()->GetHeight();
	}
	else
	{
		width = node.GetIntProperty("width", 64);
		height = node.GetIntProperty("height", 64);
	}
	m_Width = width;
	m_Height = height;

	m_CreateDepthStencilBuffer = node.GetBoolProperty("create_depth_stencil_buffer", false, false);

	static std::map<std::string, DXGI_FORMAT> formats = {
		{ "A8R8G8B8", DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM },
		{ "A32R32G32B32", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT },
		{ "A32R32G32B32F", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ "R32F", DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT }
	};

	std::string fmt = node.GetPszProperty("format_type", "", false);

	auto it = formats.find(fmt);
	if (it != formats.end())
	{
		m_format = it->second;
	}
	else
	{
		//DEBUG_ASSERT(!"Using default value!");
		m_format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	Init();
}

CDynamicTexture::~CDynamicTexture()
{
}

void CDynamicTexture::Init()
{
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	D3D11_TEXTURE2D_DESC l_TextureDescription;
	ZeroMemory(&l_TextureDescription, sizeof(D3D11_TEXTURE2D_DESC));
	l_TextureDescription.Width = m_Width;
	l_TextureDescription.Height = m_Height;
	l_TextureDescription.MipLevels = 1;
	l_TextureDescription.ArraySize = 1;
	l_TextureDescription.Format = m_format;
	l_TextureDescription.SampleDesc.Count = 1;
	l_TextureDescription.Usage = D3D11_USAGE_DEFAULT;
	l_TextureDescription.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	l_TextureDescription.CPUAccessFlags = 0;
	l_TextureDescription.MiscFlags = 0;
	HRESULT l_HR = l_Device->CreateTexture2D(&l_TextureDescription, NULL, &m_RenderTargetTexture);
	DEBUG_ASSERT(!FAILED(l_HR));
	D3D11_RENDER_TARGET_VIEW_DESC l_RenderTargetViewDescription;
	l_RenderTargetViewDescription.Format = l_TextureDescription.Format;
	l_RenderTargetViewDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	l_RenderTargetViewDescription.Texture2D.MipSlice = 0;
	l_HR = l_Device->CreateRenderTargetView(m_RenderTargetTexture,
		&l_RenderTargetViewDescription, &m_RenderTargetView);
	DEBUG_ASSERT(!FAILED(l_HR));
	D3D11_SHADER_RESOURCE_VIEW_DESC l_ShaderResourceViewDescription;
	l_ShaderResourceViewDescription.Format = l_TextureDescription.Format;
	l_ShaderResourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	l_ShaderResourceViewDescription.Texture2D.MostDetailedMip = 0;
	l_ShaderResourceViewDescription.Texture2D.MipLevels = 1;
	l_HR = l_Device->CreateShaderResourceView(m_RenderTargetTexture, &l_ShaderResourceViewDescription, GetShaderResourceView());
	DEBUG_ASSERT(!FAILED(l_HR));
	if (m_CreateDepthStencilBuffer)
	{
		D3D11_TEXTURE2D_DESC l_DepthBufferDescription;
		ZeroMemory(&l_DepthBufferDescription, sizeof(D3D11_TEXTURE2D_DESC));
		l_DepthBufferDescription.Width = m_Width;
		l_DepthBufferDescription.Height = m_Height;
		l_DepthBufferDescription.MipLevels = 1;
		l_DepthBufferDescription.ArraySize = 1;
		l_DepthBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		l_DepthBufferDescription.SampleDesc.Count = 1;
		l_DepthBufferDescription.SampleDesc.Quality = 0;
		l_DepthBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		l_DepthBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		l_DepthBufferDescription.CPUAccessFlags = 0;
		l_DepthBufferDescription.MiscFlags = 0;
		l_HR = l_Device->CreateTexture2D(&l_DepthBufferDescription, NULL, &m_DepthStencilBuffer);
		DEBUG_ASSERT(!FAILED(l_HR));
		D3D11_DEPTH_STENCIL_VIEW_DESC l_DepthStencilViewDescription;
		ZeroMemory(&l_DepthStencilViewDescription, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		l_DepthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		l_DepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		l_DepthStencilViewDescription.Texture2D.MipSlice = 0;
		l_HR = l_Device->CreateDepthStencilView(m_DepthStencilBuffer, &l_DepthStencilViewDescription, &m_DepthStencilView);
		DEBUG_ASSERT(!FAILED(l_HR));
	}
	CreateSamplerState();
}

bool CDynamicTexture::CreateSamplerState()
{
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
	l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD = 0;
	l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT l_HR = l_Device->CreateSamplerState(&l_SampDesc, GetSamplerState());
	return !FAILED(l_HR);
}
