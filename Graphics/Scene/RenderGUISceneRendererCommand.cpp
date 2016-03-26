#include "Scene/RenderGUISceneRendererCommand.h"
#include <Core/Engine/Engine.h>
#include "Context/ContextManager.h"
#include "Material/MaterialManager.h"
#include <GUI/GUI.h>

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
{
	m_GUI = new CGUI();
	m_GUI->Init(CEngine::GetSingleton().getContextManager());
}

void CRenderGUISceneRendererCommand::Execute(CContextManager &_context)
{
	m_GUI->BeginGUI();

	m_GUI->Image( Rectf( 50, 50, 200, 50 ), "test-gui-material", 0, CGUI::Alignment::CENTER, CGUI::Alignment::CENTER);


	m_GUI->BeginFrame( Rectf( 0, 0, 400, 300 ), CGUI::Alignment::CENTER );

	m_GUI->Image( Rectf( 50, 50, 200, 50 ), "test-gui-material", 0, CGUI::Alignment::CENTER, CGUI::Alignment::CENTER);

	m_GUI->EndFrame();


	m_GUI->BeginFrame( Rectf( 0.5, 0, 0.5, 0.5, true ) );

	m_GUI->Image( Rectf( 50, 50, 200, 50 ), "test-gui-material", 0, CGUI::Alignment::CENTER, CGUI::Alignment::CENTER);

	m_GUI->EndFrame();


	m_GUI->EndGUI();
}
