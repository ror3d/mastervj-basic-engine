#include "Scene/SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"

CSetPoolRenderableObjectsTechniqueSceneRendererCommand::CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode){
	m_Pool->setName(TreeNode.GetPszProperty("name"));
}

void CSetPoolRenderableObjectsTechniqueSceneRendererCommand::Execute(CContextManager &_context){
	//Como añade un elemento a RenderableObjectsTEchManAGER->pOOLS??
}