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

	TMapManager<CTexture>::Ref GetTexture(const std::string &Filename);
	void Reload();

	void SetTextureAtStage(int stage, CTexture* texture) { m_activeTextures[stage] = texture; }
	void DeactivateTextures();
	bool isEmpty() { return (m_resources.size() == 0); }
};

#endif