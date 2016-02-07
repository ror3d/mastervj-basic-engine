#include "Scene/RenderDebugGridSceneRendererCommand.h"
#include "Engine/Engine.h"

CRenderDebugGridSceneRendererCommand::CRenderDebugGridSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CSceneRendererCommand(TreeNode){

}

void CRenderDebugGridSceneRendererCommand::Execute(CContextManager &_context){
	//_context->Draw(CEngine::GetSingleton().getDebugRender()->GetSimpleCube());
}