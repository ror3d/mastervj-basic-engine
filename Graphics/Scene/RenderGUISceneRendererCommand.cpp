#include "Scene/RenderGUISceneRendererCommand.h"
#include <Core/Engine/Engine.h>
#include "Context/ContextManager.h"
#include "Material/MaterialManager.h"
#include <GUI/GUI.h>
#include <Core/Input/InputManager.h>

#include <Base/Scripting/ScriptManager.h>

#pragma execution_character_set("utf-8")

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
{
	m_GUI = new CGUI();
	m_GUI->Init("Data\\gui_elements.xml");
}

void CRenderGUISceneRendererCommand::Execute(CContextManager &_context)
{
	CGUI::SetInstance(m_GUI);
	m_GUI->BeginGUI();
	CEngine::GetSingleton().getScriptManager()->RunScript("gui");
	m_GUI->EndGUI();
	// TODO call scripts to render gui
	/*
	m_GUI->BeginGUI();

	m_GUI->Image( "btn-test-i", Rectf( 50, 50, 200, 50 ), Rectf::Alignment::CENTER, Rectf::Alignment::CENTER);


	m_GUI->BeginFrame( Rectf( 0, 0, 400, 300 ), Rectf::Alignment::CENTER );

	m_GUI->Image( "btn-test-i", Rectf( 50, 50, 200, 50 ), Rectf::Alignment::CENTER, Rectf::Alignment::CENTER);

	m_GUI->EndFrame();


	m_GUI->BeginFrame( Rectf( 0.5, 0, 0.5, 0.5, true ) );

	m_GUI->Image( "btn-test-o", Rectf( 50, 50, 200, 50 ), Rectf::Alignment::CENTER, Rectf::Alignment::CENTER);

	m_GUI->BeginFrame( Rectf( 0.5, 0, 0.5, 0.5, true ) );

	m_GUI->Image( "btn-test-p", Rectf( 50, 50, 200, 50 ), Rectf::Alignment::CENTER, Rectf::Alignment::CENTER);

	m_GUI->EndFrame();
	m_GUI->EndFrame();

	m_GUI->Button("btn-test", Rectf(0, 0, 200, 50), Rectf::Alignment::TOP_LEFT, Rectf::Alignment::TOP_LEFT);

	Vect2i mouse = CInputManager::GetInputManager()->GetCursor();

	m_GUI->Image("btn-test-p", Rectf(mouse.x, mouse.y, 4, 4), Rectf::Alignment::TOP_LEFT, Rectf::Alignment::CENTER);


	m_GUI->BeginFrame(Rectf(0, 0, 300, 300), Rectf::Alignment::CENTER, Rectf::Alignment::CENTER);
		m_GUI->Image( "btn-test-i", Rectf( 0, 0, 1, 1, true ), Rectf::Alignment::TOP_LEFT, Rectf::Alignment::TOP_LEFT);
	m_GUI->EndFrame();

	m_GUI->Text("Arial32", "hola què tal com anem?\naixò hauria de ser una altra línia.", Rectf(0, 0, 300, 300), Rectf::Alignment::CENTER, Rectf::Alignment::TOP_RIGHT);

	m_GUI->EndGUI();
	*/
}
