#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <Base/Utils/TMapManager.h>
#include <Base/Math/Color.h>

class CRenderManager;
class CLight;
class CContextManager;

class CTexture;

class CLightManager : public TMapManager<CLight>
{
private:
	std::string m_FileName;

	CColor m_ambient;
public:
	CLightManager();
	virtual ~CLightManager();
	void Load(const std::string &FileName);
	void Render(CRenderManager *RenderManager);
	CLight& CLightManager::iterate(size_t id);
	size_t CLightManager::count();
	void ExecuteShadowCreation(CContextManager &_context);
	void reload();

	inline CColor GetAmbientLight() { return m_ambient; }
};

#endif