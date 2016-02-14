#include "TextureManager.h"


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
		texture->load(Filename);
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