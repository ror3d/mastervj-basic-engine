#include "Scene\SetMatricesSceneRendererCommand.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode){

}
void CSetMatricesSceneRendererCommand::Execute(CContextManager &_context){
	//Llamar a CApplication::Render()?
	//Mover camaras a RenderManager, para sacar de Application??
	//_context.SetCamerasMatrix();
}
