#include "CameraManager.h"

#include "Camera.h"
#include "Frustum.h"

#include "SphericalCameraController.h"
#include "FPSCameraController.h"

#include <Base/Math/Math.h>

#include <Core/Engine/Engine.h>



CCameraManager::CCameraManager(CContextManager* context)
	: m_context(context)
{
	auto spheriCam = new CSphericalCameraController();
	spheriCam->SetZoom( 5 );
	add( "__debug",  spheriCam);
	add( "__fps", new CFPSCameraController() );

	m_currentCameraControllerName = "__fps";
}

CCameraManager::~CCameraManager()
{
	destroy();
}

void CCameraManager::Load( const std::string & file )
{
	// TODO
}

void CCameraManager::Update( float ElapsedTime )
{
	auto cc = get( m_currentCameraControllerName );
	cc->Update( ElapsedTime );
}

void CCameraManager::SetMatrixes()
{
	CCameraController* cc = get(m_currentCameraControllerName);
	CCamera l_Camera;
	l_Camera.SetAspectRatio( m_context->GetAspectRatio() );
	cc->UpdateCameraValues( &l_Camera );

	Mat44f viewProj = l_Camera.GetView() * l_Camera.GetProjection();
	m_CullFrustum.Update(viewProj);

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

}
