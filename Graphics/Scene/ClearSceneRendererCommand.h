#ifndef CLEAR_SCENE_RENDERER_COMMAND_H
#define CLEAR_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CClearSceneRendererCommand : public CSceneRendererCommand
{
protected:
	bool m_RenderTarget;
	bool m_DepthStencil;
public:
	CClearSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CContextManager &_contex);
};

#endif