#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <Utils/TMapManager.h>

class CRenderManager;
class CLight;

class CLightManager : public TMapManager<CLight>
{
private:
	std::string m_FileName;
public:
	CLightManager();
	virtual ~CLightManager();
	void Load(const std::string &FileName);
	void Render(CRenderManager *RenderManager);
	CLight& CLightManager::iterate(size_t id);
	size_t CLightManager::count();
	void reload();
};

#endif