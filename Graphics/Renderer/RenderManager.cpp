#include "RenderManager.h"

#include "Context/ContextManager.h"
#include "Material/MaterialManager.h"
#include "Renderable/RenderableObjectsManager.h"
#include <Engine/Engine.h>


CRenderManager::CRenderManager()
	: m_UseDebugCamera(false)
{
	m_RenderableObjects = CEngine::GetSingleton().getRenderableObjectManager();
}


void CRenderManager::SetCurrentCamera(const CCamera& _CurrentCamera)
{
	m_CurrentCamera = _CurrentCamera;
	Mat44f viewProj = m_CurrentCamera.GetView() * m_CurrentCamera.GetProjection();
	m_CullFrustum.Update(viewProj);
}


void CRenderManager::Render(CContextManager* _Context)
{
	CCamera &l_Camera = (m_UseDebugCamera ? m_DebugCamera : m_CurrentCamera);

	CEffectManager::m_SceneParameters.m_CameraPosition = l_Camera.GetPosition();
	CEffectManager::m_SceneParameters.m_CameraRightVector = l_Camera.GetLookAt();
	CEffectManager::m_SceneParameters.m_CameraUpVector = l_Camera.GetUp();
	auto view = l_Camera.GetView();
	CEffectManager::m_SceneParameters.m_View = view;
	view.Invert();
	CEffectManager::m_SceneParameters.m_ViewInverse = view;
	CEffectManager::m_SceneParameters.m_Projection = l_Camera.GetProjection();

	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[0] = l_Camera.GetZNear();
	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[1] = l_Camera.GetZFar();
	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[2] = l_Camera.GetFOV();
	CEffectManager::m_SceneParameters.m_CameraProjectionInfo[3] = l_Camera.GetAspectRatio();

	CEngine::GetSingleton().getEffectsManager()->SetLightsConstants();

	m_RenderableObjects->Render(_Context);
}
