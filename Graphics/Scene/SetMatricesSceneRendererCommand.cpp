#include "Scene/SetMatricesSceneRendererCommand.h"

#include "Engine/Engine.h"
#include <Graphics/Camera/CameraManager.h>

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode)
{
}

void CSetMatricesSceneRendererCommand::Execute(CContextManager &_context)
{
	CEngine::GetSingleton().getCameraManager()->SetMatrixes();
}
