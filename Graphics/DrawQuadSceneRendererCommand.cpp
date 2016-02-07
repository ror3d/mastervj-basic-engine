#include "Scene/DrawQuadSceneRendererCommand.h"
#include "Engine/Engine.h"

CDrawQuadSceneRendererCommand::CDrawQuadSceneRendererCommand(CXMLTreeNode &TreeNode) 
	: CStagedTexturedSceneRendererCommand(TreeNode){
	m_RenderableObjectTechnique = CEngine::GetSingleton().getMaterialManager()->get(TreeNode.GetPszProperty("material"))->getRenderableObjectTechique();
	for (int i = 0; i < TreeNode.GetNumChildren(); i++){
		CXMLTreeNode textChild = TreeNode(i);
		CTexture * text = new CTexture();
		text->setName(textChild.GetPszProperty("file"));
		AddStageTexture(textChild.GetIntProperty("stage_id"), text);
	}
	/*<render_draw_quad material="DeferredFogMaterial">
	<texture stage_id="0" file="DeferredCapturedTexture"/>
	<texture stage_id="1" file="DepthMapTexture"/>
	</render_draw_quad>*/
}

void CDrawQuadSceneRendererCommand::Execute(CContextManager &_context){

}