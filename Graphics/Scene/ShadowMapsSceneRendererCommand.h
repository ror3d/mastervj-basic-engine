#ifndef	GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H	
#define GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CGenerateShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CGenerateShadowMapsSceneRendererCommand();
	void Execute(CContextManager &_context);
};

#endif