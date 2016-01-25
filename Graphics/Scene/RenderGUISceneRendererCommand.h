#ifndef RENDER_GUI_SCENE_RENDERER_COMMAND_H
#define RENDER_GUI_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CRenderGUISceneRendererCommand : public CSceneRendererCommand
{
private:
	//TODO: Game GUI
	//CGUI *m_GUI;
public:
	CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CContextManager &_context);
};

#endif