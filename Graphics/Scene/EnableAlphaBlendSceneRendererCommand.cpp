#include "Scene\EnableAlphaBlendSceneRendererCommand.h"
#include "Context/ContextManager.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
{ 

}

void CEnableAlphaBlendSceneRendererCommand::Execute(CContextManager &_context)
{
	_context.EnableAlphaBlendState();
}