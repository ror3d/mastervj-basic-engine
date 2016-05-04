#include "Scene/RenderDebugLayerSceneRendererCommand.h"
#include <Core/Engine/Engine.h>
#include <Graphics/Debug/DebugRender.h>

CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode)
	:CSceneRendererCommand(TreeNode)
{
}

void CRenderDebugLayerSceneRendererCommand::Execute(CContextManager &_context)
{
	CEngine::GetSingleton().getDebugRender()->Render();
}