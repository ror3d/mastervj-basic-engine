#ifndef APPLICATION_H
#define APPLICATION_H

#include <Camera/SphericalCameraController.h>
#include <Camera/FPSCameraController.h>
#include "HelperTypes.h"
#include <Material/MaterialManager.h>

class CDebugRender;
class CContextManager;
class CRenderManager;

class CApplication
{
public:
	CApplication( CContextManager *_ContextManager, CRenderManager *_renderManager );
	~CApplication();

	void Init();

	void SwitchCamera();

	void Update(float _ElapsedTime);
	void Render();

	void Destroy() {/* TODO */}
private:

	CSphericalCameraController m_SphericalCamera;
	CFPSCameraController m_FPSCamera;

	int m_CurrentCamera;

	CDebugRender *m_DebugRender;
	CContextManager *m_ContextManager;
	CRenderManager *m_RenderManager;

	CColor m_BackgroundColor;

};

#endif
