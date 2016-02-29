#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <Base/Utils/TMapManager.h>
#include "Texture.h"

class CTextureManager : public TMapManager<CTexture>
{
	std::map<int, CTexture*> m_activeTextures;
public:
	CTextureManager();
	virtual ~CTextureManager();

	CTexture * GetTexture(const std::string &Filename);
	void Reload();

	void SetTextureAtStage(int stage, CTexture* texture) { m_activeTextures[stage] = texture; }
	CTexture* GetTextureAtStage(int stage) { return m_activeTextures[stage]; }
	void DeactivateTextures();
};

#endif