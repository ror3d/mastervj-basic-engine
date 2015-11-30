#include "Texture.h"

#include "Context/ContextManager.h"
#include <d3d11.h>

CTexture::CTexture()
{
}


CTexture::~CTexture()
{
}


bool CTexture::LoadFile()
{
	/* TODO
	ID3D11Device *l_Device = UABEngine.GetRenderManager().GetDevice();
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
	//*/
	return false;
}

void CTexture::Unload()
{
}

bool CTexture::load(const std::string &Filename)
{
	return false;
}

void CTexture::Activate(unsigned int StageId)
{
	/* TODO
	ID3D11DeviceContext
		*l_DeviceContext = UABEngine.GetInstance().GetRenderManager().GetDeviceContext();
	l_DeviceContext->PSSetSamplers(StageId, 1, &m_SamplerState);
	l_DeviceContext->PSSetShaderResources(StageId, 1, &m_Texture);
	//*/
}

bool CTexture::Reload()
{
	return false;
}

