#include "TextureManager.h"
#include <Core/Engine/Engine.h>
#include <Graphics/Context/ContextManager.h>


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
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
