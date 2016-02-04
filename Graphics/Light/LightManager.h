#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <Utils/TMapManager.h>

class CRenderManager;
class CLight;

class CLightManager : public TMapManager<CLight>
{
public:
	CLightManager();
	virtual ~CLightManager();
	void Load(const std::string &FileName);
	void Render(CRenderManager *RenderManager);
};

#endif