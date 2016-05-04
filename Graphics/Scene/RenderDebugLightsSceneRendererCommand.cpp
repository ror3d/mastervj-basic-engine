#include "Scene/RenderDebugLightsSceneRendererCommand.h"
#include "Engine/Engine.h"
#include <Graphics/Effect/EffectManager.h>

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &TreeNode)
	:CSceneRendererCommand(TreeNode)
{

}

void CRenderDebugLightsSceneRendererCommand::Execute(CContextManager &_context)
{
	CEngine::GetSingleton().getEffectsManager()->SetLightsConstants();
}