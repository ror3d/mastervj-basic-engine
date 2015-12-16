#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Camera/Camera.h"
#include "Camera/Frustum.h"

class CRenderableObjectsManager;
class CContextManager;
class CMaterialManager;

class CRenderManager
{
public:
	CRenderManager();

	void SetCurrentCamera(const CCamera& _CurrentCamera);
	void SetDebugCamera(const CCamera& _DebugCamera) { m_DebugCamera = _DebugCamera; }
	void SetUseDebugCamera(bool _use) { m_UseDebugCamera = _use; }

	void Render(CContextManager* _Context);

	void destroy() {}

private:

	CCamera m_CurrentCamera;
	CFrustum m_CullFrustum;
	CCamera m_DebugCamera;
	bool m_UseDebugCamera;

	CRenderableObjectsManager * m_RenderableObjects;
};

#endif