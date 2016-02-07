#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Camera/Camera.h"
#include "Camera/Frustum.h"
#include <Camera/SphericalCameraController.h>
#include <Camera/FPSCameraController.h>

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
	int getCurrentCameraNum(){ return m_CurrentCameraNum; }
	CSphericalCameraController getSphericalCamera(){ return m_SphericalCamera; }
	CFPSCameraController getFPSCamera(){ return m_FPSCamera; }
	CCamera getCurrentCamera(){ return m_CurrentCamera;  }

	void SetCamerasMatrix(CContextManager *_context);
	void SwitchCamera();
	void Render();

	void destroy() {}

private:
	CSphericalCameraController m_SphericalCamera;
	CFPSCameraController m_FPSCamera;
	int m_CurrentCameraNum;
	CCamera m_CurrentCamera;
	CCamera m_DebugCamera;
	CFrustum m_CullFrustum;
	
	bool m_UseDebugCamera;
};

#endif