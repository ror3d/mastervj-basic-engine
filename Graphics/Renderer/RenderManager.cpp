#include "RenderManager.h"

#include "Context/ContextManager.h"
#include "Material/MaterialManager.h"
#include "RenderableObjectsManager.h"
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
	l_Camera.SetPosition(Vect3f(6, 0, 0));
	l_Camera.SetMatrixs();
	CEffectManager::m_Parameters.m_View=l_Camera.GetView();
	CEffectManager::m_Parameters.m_Projection= l_Camera.GetProjection();

	m_RenderableObjects->Render(_Context);

}
