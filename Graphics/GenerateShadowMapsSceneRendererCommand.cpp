#include "Scene\GenerateShadowMapsSceneRendererCommand.h"
#include "Engine/Engine.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode) 
	:CSceneRendererCommand(TreeNode){

}

CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand(){

}


void CGenerateShadowMapsSceneRendererCommand::Execute(CContextManager &_context){
	/*El método Execute recorrerá las luces del lightmanager y comprobará si la luz ha
	de generar ShadowMap y si está activa, en ese caso llamará al método
	SetShadowMap de la luz que establecerá las matrices de View y Projección,
	realizará el clear del zbuffer y del stencil buffer y pintará las capas que el
	shadowmap tenga aplicadas.	*/
	CEngine::GetSingleton().getLightManager()->ExecuteShadowCreation(_context);
}
