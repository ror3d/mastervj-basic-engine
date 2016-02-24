#include "Scene/CaptureFrameBufferSceneRendererCommand.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CStagedTexturedSceneRendererCommand(TreeNode){
	/*<capture_frame_buffer>
		<capture_texture name = "DeferredCapturedTexture"	texture_width_as_frame_buffer = "true" format_type = "A8R8G8B8" / >
		< / capture_frame_buffer>*/
	for (int i = 0; i < TreeNode.GetNumChildren(); i++){
		CXMLTreeNode stagedTextNode = TreeNode(i);
		if (stagedTextNode.GetName() == std::string("capture_texture")){
			CTexture * text = new CTexture();
			text->setName(stagedTextNode.GetPszProperty("name"));
			//TODO FORMAT & TEXTUREWIDTH....
			CStagedTexturedSceneRendererCommand::CStageTexture * stagedText = new CStagedTexturedSceneRendererCommand::CStageTexture(i, text);
			m_StageTextures.push_back(*stagedText);
		}
	}
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CContextManager &_context){

}