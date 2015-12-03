#include "TextureManager.h"


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
}


CTexture * CTextureManager::GetTexture(const std::string &Filename)
{
	return get(Filename);
}


void CTextureManager::Reload()
{
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Reload();
	}
}