#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <Utils/TMapManager.h>

class CRenderManager;
class CLight;
class CContextManager;

class CLightManager : public TMapManager<CLight>
{
public:
	CLightManager();
	virtual ~CLightManager();
	void Load(const std::string &FileName);
	void Render(CRenderManager *RenderManager);
	CLight& CLightManager::iterate(size_t id);
	size_t CLightManager::count();
	void ExecuteShadowCreation(CContextManager &_context);
};

#endif