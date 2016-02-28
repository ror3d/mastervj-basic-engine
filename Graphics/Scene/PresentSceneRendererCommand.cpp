#include "Scene/PresentSceneRendererCommand.h"
#include "Context/ContextManager.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}

void CPresentSceneRendererCommand::Execute(CContextManager &_context)
{
	_context.Present();
}