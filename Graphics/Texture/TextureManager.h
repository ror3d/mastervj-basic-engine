#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <Utils/MapManager.h>
#include "Texture.h"

class CTextureManager : public TemplatedMapManager<CTexture>
{
public:
	CTextureManager();
	virtual ~CTextureManager();

	CTexture * GetTexture(const std::string &Filename);
	void Reload();
};

#endif