#ifndef RENDER_GUI_SCENE_RENDERER_COMMAND_H
#define RENDER_GUI_SCENE_RENDERER_COMMAND_H

#include "Scene/SceneRendererCommand.h"

class CGUI;

class CRenderGUISceneRendererCommand : public CSceneRendererCommand
{
private:
	CGUI *m_GUI;
public:
	CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode);
	~CRenderGUISceneRendererCommand();
	virtual void Execute(CContextManager &_context);
};

#endif