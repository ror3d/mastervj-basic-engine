#ifndef CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H
#define CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H

#include "Scene\StagedTexturedSceneRendererCommand.h"

class CCaptureFrameBufferSceneRendererCommand : public
	CStagedTexturedSceneRendererCommand
{
public:
	CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CContextManager &_context);
};

#endif