#pragma once
#include <Utils/MapManager.h>
#include "Texture.h"

class CTextureManager : public TemplatedMapManager<CTexture>
{
public:
	CTextureManager();
	~CTextureManager();
};

