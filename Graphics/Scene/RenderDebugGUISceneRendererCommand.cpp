#include "RenderDebugGUISceneRendererCommand.h"
#include "Debug/DebugHelper.h"

CRenderDebugGUISceneRendererCommand::CRenderDebugGUISceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode){
}

void CRenderDebugGUISceneRendererCommand::Execute(CContextManager &_contex){
	//Llamar anttweak RenderDebug
	CDebugHelper::GetDebugHelper()->Render();
}