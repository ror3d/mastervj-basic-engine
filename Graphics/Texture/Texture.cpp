#include "Texture.h"

#include "Context/ContextManager.h"
#include <Graphics/Texture/TextureManager.h>
#include <Engine/Engine.h>
#include <d3dx11.h>




CTexture::CTexture() : CNamed("")
, m_Texture(NULL)
, m_SamplerState(NULL)
{
}

CTexture::CTexture(const std::string& name) : CNamed(name)
, m_Texture(NULL)
, m_SamplerState(NULL)
{
}


CTexture::~CTexture()
{
	Unload();
}


bool CTexture::LoadFile()
{
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	HRESULT l_HR = D3DX11CreateShaderResourceViewFromFile(l_Device,
		getName().c_str(), NULL, NULL, &m_Texture, NULL);
	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
	l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD = 0;
	l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	l_HR = l_Device->CreateSamplerState(&l_SampDesc, &m_SamplerState);
	return SUCCEEDED(l_HR);
}

void CTexture::Unload()
{
	if (m_Texture != NULL)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}
	if (m_SamplerState != NULL)
	{
		m_SamplerState->Release();
		m_SamplerState = NULL;
	}
}

bool CTexture::load(const std::string &Filename)
{
	setName(Filename);
	return LoadFile();
}

void CTexture::Activate(unsigned int StageId)
{
	if (m_Texture == NULL || m_SamplerState == NULL)
		return;
	ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().getContextManager()->GetDeviceContext();
	l_DeviceContext->PSSetSamplers(StageId, 1, &m_SamplerState);
	l_DeviceContext->PSSetShaderResources(StageId, 1, &m_Texture);

	CEngine::GetSingleton().getTextureManager()->SetTextureAtStage(StageId, this);
}

bool CTexture::Reload()
{
	Unload();
	return load(getName());
}

