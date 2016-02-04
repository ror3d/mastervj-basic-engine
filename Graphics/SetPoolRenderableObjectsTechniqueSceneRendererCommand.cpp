#include "Scene/SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "Engine/Engine.h"

CSetPoolRenderableObjectsTechniqueSceneRendererCommand::CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode){
	m_Pool = * CEngine::GetSingleton().getRenderableObjectTechniqueManager()->GetPoolRenderableObjectTechniques().get(TreeNode.GetPszProperty("name"));
}

void CSetPoolRenderableObjectsTechniqueSceneRendererCommand::Execute(CContextManager &_context){
	m_Pool->Apply();
}