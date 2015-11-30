#pragma once

#include <Utils/MapManager.h>

class CMaterial;

class CMaterialManager : public TemplatedMapManager<CMaterial>
{
public:
	CMaterialManager();
	virtual ~CMaterialManager();
	void load(const std::string &Filename);
	void Reload();


};

