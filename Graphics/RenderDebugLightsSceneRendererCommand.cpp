#include "Scene/RenderDebugLightsSceneRendererCommand.h"
#include "Engine/Engine.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CSceneRendererCommand(TreeNode){

}

void CRenderDebugLightsSceneRendererCommand::Execute(CContextManager &_context){
	CEngine::GetSingleton().getEffectsManager()->SetLightsConstants();
}