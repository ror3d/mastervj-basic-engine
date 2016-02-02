#include "Scene/RenderLayerSceneRendererCommand.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CSceneRendererCommand(TreeNode){
	//<render_layer layer="solid" active="true"/>
	//CEngine::GetSingleton().getLayerManager(); y guardarnos la capa
}

void CRenderLayerSceneRendererCommand::Execute(CContextManager &_context){
	//llamar a render de la capa
}