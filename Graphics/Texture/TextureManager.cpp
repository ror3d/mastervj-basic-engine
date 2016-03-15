#include "TextureManager.h"
#include <Core/Engine/Engine.h>


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
}


TMapManager<CTexture>::Ref CTextureManager::GetTexture(const std::string &Filename)
{
	TMapManager<CTexture>::Ref texture = get(Filename);

	if (texture == nullptr)
	{
		CTexture* tex = new CTexture();
		bool success = tex->load(Filename);
		DEBUG_ASSERT( success );
		add(tex->getName(), tex);
		texture = get(Filename);
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
