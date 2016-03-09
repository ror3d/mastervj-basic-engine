#include "Scene/CaptureFrameBufferSceneRendererCommand.h"
#include "Texture/CaptureFrameBufferTexture.h"
#include <Engine/Engine.h>

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CStagedTexturedSceneRendererCommand(TreeNode){
	/*<capture_frame_buffer>
		<capture_texture name = "DeferredCapturedTexture"	texture_width_as_frame_buffer = "true" format_type = "A8R8G8B8" / >
		< / capture_frame_buffer>*/
	for (int i = 0; i < TreeNode.GetNumChildren(); i++){
		CXMLTreeNode stagedTextNode = TreeNode(i);
		if (stagedTextNode.GetName() == std::string("capture_texture"))
		{
			CCaptureFrameBufferTexture * text = new CCaptureFrameBufferTexture(stagedTextNode);
			AddStageTexture(0, text);

			CEngine::GetSingleton().getTextureManager()->add(text->getName(), text);
		}
	}
}

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
	CTextureManager * textureManager = CEngine::GetSingleton().getTextureManager();

	if (!textureManager->isEmpty())
	{
		for (auto const & text : m_StageTextures)
		{
			textureManager->remove(text.m_Texture->getName());
		}
	}
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CContextManager &_context)
{
	for (auto stex : m_StageTextures)
	{
		dynamic_cast<CCaptureFrameBufferTexture*>(stex.m_Texture)->Capture(stex.m_StageId);
	}
}