#include "TextureManager.h"
#include <Core/Engine/Engine.h>
#include <Graphics/Context/ContextManager.h>


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
	for ( auto & const ssIt : m_samplerStates )
	{
		if ( ssIt.second != NULL )
		{
			ssIt.second->Release();
			ssIt.second = NULL;
		}
	}
}

void CTextureManager::Init()
{
	CreateSamplerState( SamplerStateType::LinearFilter_ClampEdges, true, false );
	CreateSamplerState( SamplerStateType::LinearFilter_WrapEdges, true, true );
	CreateSamplerState( SamplerStateType::NoFilter_ClampEdges, false, false );
	CreateSamplerState( SamplerStateType::NoFilter_WrapEdges, false, false );
}


CTexture * CTextureManager::GetTexture(const std::string &Filename)
{
	CTexture *texture = get(Filename);

	if (texture == nullptr)
	{
		texture = new CTexture();
		bool success = texture->load(Filename);
		DEBUG_ASSERT( success );
		add(texture->getName(), texture);
	}

	return texture;
}


void CTextureManager::Reload()
{
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Reload();
	}
}

void CTextureManager::DeactivateTextures()
{
	ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().getContextManager()->GetDeviceContext();

	ID3D11SamplerState *const tabs[1] = {NULL};
	ID3D11ShaderResourceView *const tabr[1] = {NULL};

	for (auto const &tex : m_activeTextures)
	{
		if (tex.second == nullptr) continue;
		l_DeviceContext->PSSetSamplers( tex.first, 1, tabs );
		l_DeviceContext->PSSetShaderResources( tex.first, 1, tabr );
	}

	m_activeTextures.clear();
}

void CTextureManager::CreateSamplerState( SamplerStateType type, bool linearFilter, bool wrapEdges )
{
	D3D11_SAMPLER_DESC l_SampDesc;
	ZeroMemory(&l_SampDesc, sizeof(l_SampDesc));
	if (linearFilter)
	{
		l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else
	{
		l_SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
	if (wrapEdges)
	{
		l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	}
	else
	{
		l_SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		l_SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		l_SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	}
	l_SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	l_SampDesc.MinLOD = 0;
	l_SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ID3D11SamplerState *l_SamplerState;

	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	HRESULT l_HR = l_Device->CreateSamplerState(&l_SampDesc, &l_SamplerState);

	DEBUG_ASSERT( SUCCEEDED( l_HR ) );

	m_samplerStates[type] = l_SamplerState;
}
