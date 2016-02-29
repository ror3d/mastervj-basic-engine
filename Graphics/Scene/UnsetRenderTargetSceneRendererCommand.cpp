#include "Scene/UnsetRenderTargetSceneRendererCommand.h"
#include "Context/ContextManager.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CSceneRendererCommand(TreeNode){

}


void CUnsetRenderTargetSceneRendererCommand::Execute(CContextManager &_context)
{
	_context.UnsetRenderTargets();
}