#include "Scene\EnableAlphaBlendSceneRendererCommand.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
{ 

}

void CEnableAlphaBlendSceneRendererCommand::Execute(CContextManager &_context)
{
	_context.EnableAlphaBlendState();
}