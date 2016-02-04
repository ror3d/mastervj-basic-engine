#include "Application.h"

#include <Base/Math/Math.h>

#include <Graphics/Context/ContextManager.h>
#include <Graphics/Debug/DebugRender.h>
#include <Graphics/Renderer/RenderManager.h>

#include <Core/Input/InputManager.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Core/Debug/DebugHelper.h>

#include <XML/XMLTreeNode.h>
#include <Graphics/Cinematics/Cinematic.h>

#include <PhysX/PhysXManager.h>

#include <Base/Scripting/ScriptManager.h>


static float s_mouseSpeed = 1;

CScriptManager *s_sm = nullptr;

static void __stdcall SwitchCameraCallback( void* _app )
{
	( (CApplication*)_app )->SwitchCamera();
}

CApplication::CApplication( CContextManager *_ContextManager, CRenderManager *_renderManager )
	: m_RenderManager( _renderManager )
	, m_ContextManager( _ContextManager )
	, m_BackgroundColor( .2f, .1f, .4f )
	, m_CurrentCamera( 0 )
{
	CDebugHelper::GetDebugHelper()->Log( "CApplication::CApplication" );

	CDebugHelper::SDebugBar bar;
	bar.name = "CApplication";
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "background";
		var.type = CDebugHelper::COLOR;
		var.mode = CDebugHelper::READ_WRITE;
		var.pColor = &m_BackgroundColor;

		bar.variables.push_back(var);
	}
	/*
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "cube";
		var.type = CDebugHelper::POSITION_ORIENTATION;
		var.mode = CDebugHelper::READ_WRITE;
		var.pPositionOrientation = m_Cube.GetPtrTransform();

		bar.variables.push_back(var);
	}
	*/
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "switch camera";
		var.type = CDebugHelper::BUTTON;
		var.callback = SwitchCameraCallback;
		var.data = this;

		bar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "mouse speed";
		var.type = CDebugHelper::FLOAT;
		var.mode = CDebugHelper::READ_WRITE;
		var.pFloat = &s_mouseSpeed;
		var.params = " min=0.1 max=10 step=0.1 precision=1 ";

		bar.variables.push_back(var);
	}

	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
}


CApplication::~CApplication()
{
	CDebugHelper::GetDebugHelper()->Log( "CApplication::~CApplication" );
}

void CApplication::Init()
{
	s_sm = new CScriptManager();
	s_sm->Initialize();
	s_sm->RegisterLUAFunctions();
	//s_sm->RunCode("a = Vect3f.new(2, 2, 2)");
	s_sm->RunCode("v = Vect3f.new(2, 2, 2); CRenderableObjectsManager:AddMeshInstance('Teapot', 'tp2', v);");
}

void CApplication::SwitchCamera()
{
	++m_CurrentCamera;
	if ( m_CurrentCamera > 1 )
	{
		m_CurrentCamera = 0;
	}
}

void CApplication::Update( float _ElapsedTime )
{
	CEngine::GetSingleton().getRenderableObjectManager()->Update(_ElapsedTime);

	( (CInputManagerImplementation*)CInputManager::GetInputManager() )->SetMouseSpeed( s_mouseSpeed );

	switch ( m_CurrentCamera )
	{
		case 0:
			if ( CInputManager::GetInputManager()->IsActionActive( "MOVE_CAMERA" ) )
			{
				Vect3f cameraMovement( 0, 0, 0 );

				cameraMovement.x = CInputManager::GetInputManager()->GetAxis( "X_AXIS" ) * 0.0005f;
				cameraMovement.y = CInputManager::GetInputManager()->GetAxis( "Y_AXIS" ) * 0.005f;

				m_SphericalCamera.Update( cameraMovement );
			}
			break;
		case 1:
		{
			m_FPSCamera.AddYaw( -CInputManager::GetInputManager()->GetAxis( "X_AXIS" ) * 0.0005f );
			m_FPSCamera.AddPitch( CInputManager::GetInputManager()->GetAxis( "Y_AXIS" ) * 0.005f );

			m_FPSCamera.Move( CInputManager::GetInputManager()->GetAxis( "STRAFE" ), CInputManager::GetInputManager()->GetAxis( "MOVE_FWD" ), false, _ElapsedTime );
		}
		break;
	}
}

void CApplication::Render()
{
	{
		CCamera camera;
		m_FPSCamera.SetCamera( &camera );
		camera.SetFOV( 1.047f );
		camera.SetAspectRatio( m_ContextManager->GetAspectRatio() );
		camera.SetZNear( 0.1f );
		camera.SetZFar( 100.f );
		camera.SetMatrixs();
		m_RenderManager->SetCurrentCamera( camera );

		m_SphericalCamera.SetZoom( 5 );
		m_SphericalCamera.SetCamera( &camera );
		camera.SetFOV( 1.047f );
		camera.SetAspectRatio( m_ContextManager->GetAspectRatio() );
		camera.SetZNear( 0.1f );
		camera.SetZFar( 100.f );
		camera.SetMatrixs();
		m_RenderManager->SetDebugCamera( camera );

		m_RenderManager->SetUseDebugCamera( m_CurrentCamera == 0 );
	}

	m_ContextManager->BeginRender( m_BackgroundColor );

	// añadir todos los objetos que se quiere pintar
	//m_RenderManager.AddRenderableObjectToRenderList(&m_Cube);

	m_RenderManager->Render( m_ContextManager );


	//Mat44f world;

	//world.SetIdentity();
	//m_ContextManager->SetWorldMatrix(world);
	//m_ContextManager->Draw(m_DebugRender->GetAxis());

	//world.SetIdentity();
	//world.SetFromPos(10, 0, 0);
	//m_ContextManager->SetWorldMatrix(world);
	//m_ContextManager->Draw(m_DebugRender->GetClassicBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_CLASSIC);

	//world.SetIdentity();
	//world.SetFromPos(0, 0, -10);
	//m_ContextManager->SetWorldMatrix(world);
	//m_ContextManager->Draw(m_DebugRender->GetPremultBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_PREMULT);


	CDebugHelper::GetDebugHelper()->Render();

	m_ContextManager->EndRender();
}
