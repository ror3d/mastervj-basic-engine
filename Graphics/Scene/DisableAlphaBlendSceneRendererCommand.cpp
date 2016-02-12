#include "Scene\DisableAlphaBlendSceneRendererCommand.h"

CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
{
}


void CDisableAlphaBlendSceneRendererCommand::Execute(CContextManager &_context)
{
	_context.DisableAlphaBlendState();
}