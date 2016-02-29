#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include "Material.h"
#include <Utils/TMapManager.h>

class CMaterialManager : public TMapManager<CMaterial>
{
private:
	std::string m_Filename;
public:
	CMaterialManager();
	virtual ~CMaterialManager();
	void load(const std::string &Filename);
	void reload();
	std::map<std::string, CMaterial*> * getMaterialsMap(){ return &m_resources; }

};

#endif