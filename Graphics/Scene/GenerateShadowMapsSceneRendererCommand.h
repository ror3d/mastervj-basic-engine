#ifndef GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "Scene\SceneRendererCommand.h"

class CGenerateShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CGenerateShadowMapsSceneRendererCommand();
	void Execute(CContextManager &_context);
	/*El método Execute recorrerá las luces del lightmanager y comprobará si la luz ha
de generar ShadowMap y si está activa, en ese caso llamará al método
SetShadowMap de la luz que establecerá las matrices de View y Projección,
realizará el clear del zbuffer y del stencil buffer y pintará las capas que el
shadowmap tenga aplicadas.*/
};

#endif