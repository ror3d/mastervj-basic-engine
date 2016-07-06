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


bool CTexture::LoadFile(bool linearFilter /*= true*/, bool wrap /*= true*/)
{
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	HRESULT l_HR = D3DX11CreateShaderResourceViewFromFile(l_Device,
		getName().c_str(), NULL, NULL, &m_Texture, NULL);

	CTextureManager::SamplerStateType type;
	if ( linearFilter )
	{
		if ( wrap )
		{
			type = CTextureManager::SamplerStateType::LinearFilter_WrapEdges;
		}
		else
		{
			type = CTextureManager::SamplerStateType::LinearFilter_ClampEdges;
		}
	}
	else
	{
		if ( wrap )
		{
			type = CTextureManager::SamplerStateType::NoFilter_WrapEdges;
		}
		else
		{
			type = CTextureManager::SamplerStateType::NoFilter_ClampEdges;
		}
	}
	m_SamplerState = CEngine::GetSingleton().getTextureManager()->GetSamplerState(type);

	return SUCCEEDED(l_HR);
}

void CTexture::Unload()
{
	if (m_Texture != NULL)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}
}

bool CTexture::load(const std::string &Filename, bool linearFilter /*= true*/, bool wrap /*= true*/)
{
	setName(Filename);
	return LoadFile(linearFilter, wrap);
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

