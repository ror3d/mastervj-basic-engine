#include "CaptureFrameBufferTexture.h"
#include "Context/ContextManager.h"
#include "Engine/Engine.h"
#include "TextureManager.h"
#include <Base/XML/XMLTreeNode.h>


CCaptureFrameBufferTexture::CCaptureFrameBufferTexture(const CXMLTreeNode &node)
	: CTexture(node.GetPszProperty("name"))
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
	Init(width, height);
}

CCaptureFrameBufferTexture::CCaptureFrameBufferTexture(const std::string &Name, unsigned int Width, unsigned int Height)
	: CTexture(Name)
{
	Init(Width, Height);
}

void CCaptureFrameBufferTexture::Init(unsigned int Width, unsigned int Height)
{
	m_Width = Width;
	m_Height = Height;
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	D3D11_TEXTURE2D_DESC l_Texture2DDescription;
	l_Texture2DDescription.Width = Width;
	l_Texture2DDescription.Height = Height;
	l_Texture2DDescription.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	l_Texture2DDescription.ArraySize = 1;
	l_Texture2DDescription.MiscFlags = 0;
	l_Texture2DDescription.MipLevels = 1;
	l_Texture2DDescription.CPUAccessFlags = 0;
	l_Texture2DDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	l_Texture2DDescription.SampleDesc.Count = 1;
	l_Texture2DDescription.SampleDesc.Quality = 0;
	l_Texture2DDescription.Usage = D3D11_USAGE_DEFAULT;
	HRESULT l_HR = l_Device->CreateTexture2D(&l_Texture2DDescription, NULL,
		&m_DataTexture);
	if (FAILED(l_HR))
		return;
	D3D11_SHADER_RESOURCE_VIEW_DESC DescRV;
	ZeroMemory(&DescRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	DescRV.Format = l_Texture2DDescription.Format;
	DescRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	DescRV.Texture2D.MipLevels = l_Texture2DDescription.MipLevels;
	DescRV.Texture2D.MostDetailedMip = 0;
	l_HR = l_Device->CreateShaderResourceView(m_DataTexture, &DescRV, GetShaderResourceView());
	if (FAILED(l_HR))
		return;

	SetSamplerState( CEngine::GetSingleton().getTextureManager()->GetSamplerState( CTextureManager::SamplerStateType::LinearFilter_ClampEdges ) );
}

CCaptureFrameBufferTexture::~CCaptureFrameBufferTexture()
{
	if (m_DataTexture != nullptr)
	{
		m_DataTexture->Release();
		m_DataTexture = nullptr;
	}
}

bool CCaptureFrameBufferTexture::Capture(unsigned int StageId)
{
	ID3D11Texture2D *l_Surface = NULL;
	HRESULT l_HR = CEngine::GetSingleton().getContextManager()->GetSwapChain()->GetBuffer(StageId, __uuidof(ID3D11Texture2D), reinterpret_cast< void** >(&l_Surface));
	if (FAILED(l_HR) || l_Surface == NULL || m_DataTexture == NULL)
		return false;
	CEngine::GetSingleton().getContextManager()->GetDeviceContext()->CopyResource(m_DataTexture, l_Surface);
	l_Surface->Release();
	return true;
}

