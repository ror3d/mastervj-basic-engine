#ifndef SET_DEPTH_STENCIL_STATE_SCENE_RENDERER_COMMAND_H
#define SET_DEPTH_STENCIL_STATE_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CSetDepthStencilStateSceneRendererCommand : public CSceneRendererCommand
{
private:
	ID3D11DepthStencilState *m_DepthStencilState;
public:
	CSetDepthStencilStateSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CSetDepthStencilStateSceneRendererCommand();
	void Execute(CContextManager &_context);
};

#endif