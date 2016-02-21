#include "RenderManager.h"

#include "Context/ContextManager.h"
#include "Material/MaterialManager.h"
#include "Renderable/RenderableObjectsManager.h"
#include <Engine/Engine.h>

CRenderManager::CRenderManager()
	: m_UseDebugCamera(false)
	, m_CurrentCameraNum(1)
{
}

void CRenderManager::SetCurrentCamera(const CCamera& _CurrentCamera)
{
	m_CurrentCamera = _CurrentCamera;
	Mat44f viewProj = m_CurrentCamera.GetView() * m_CurrentCamera.GetProjection();
	m_CullFrustum.Update(viewProj);
}

void CRenderManager::SetCamerasMatrix(CContextManager *_context){
	CCamera camera;
	m_FPSCamera.SetCamera(&camera);
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(_context->GetAspectRatio());
	camera.SetZNear(0.1f);
	camera.SetZFar(100.f);
	camera.SetMatrixs();
	SetCurrentCamera(camera);

	m_SphericalCamera.SetZoom(5);
	m_SphericalCamera.SetCamera(&camera);
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(_context->GetAspectRatio());
	camera.SetZNear(0.1f);
	camera.SetZFar(100.f);
	camera.SetMatrixs();
	SetDebugCamera(camera);

	SetUseDebugCamera(m_CurrentCameraNum == 0);	
}

void CRenderManager::SetMatrixes()
{
	CCamera &l_Camera = m_CurrentCamera;// ( m_UseDebugCamera ? m_DebugCamera : m_CurrentCamera );

	CEffectManager::m_SceneParameters.m_CameraPosition = l_Camera.GetPosition();
	CEffectManager::m_SceneParameters.m_CameraRightVector = l_Camera.GetLookAt();
	CEffectManager::m_SceneParameters.m_CameraUpVector = l_Camera.GetUp();

	CEffectManager::m_SceneParameters.m_View = l_Camera.GetView();

	CEffectManager::m_SceneParameters.m_ViewInverse = l_Camera.GetView();
	CEffectManager::m_SceneParameters.m_ViewInverse.Invert();

	CEffectManager::m_SceneParameters.m_Projection = l_Camera.GetProjection();

	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[0] = l_Camera.GetZNear();
	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[1] = l_Camera.GetZFar();
	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[2] = l_Camera.GetFOV();
	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[3] = l_Camera.GetAspectRatio();

	CEngine::GetSingleton().getEffectsManager()->SetLightsConstants();
	CEffectManager::m_SceneParameters.m_ProjectionInverse = l_Camera.GetProjection();
	CEffectManager::m_SceneParameters.m_ProjectionInverse.Invert();

	// TODO: Move somewhere else!
	CEffectManager::m_SceneParameters.m_LightAmbient = Vect4f(0.1f, 0.1f, 0.1f, 1.0f);
}

void CRenderManager::SwitchCamera()
{
	++m_CurrentCameraNum;
	if (m_CurrentCameraNum > 1)
	{
		m_CurrentCameraNum = 0;
	}
}
