#ifndef DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H
#define DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H

#include "Scene/StagedTexturedSceneRendererCommand.h"

class CDeferredShadingSceneRendererCommand : public
	CStagedTexturedSceneRendererCommand
{
private:
	ID3D11BlendState *m_EnabledAlphaBlendState;
public:
	CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CDeferredShadingSceneRendererCommand();
	void Execute(CContextManager &_context);
};

#endif