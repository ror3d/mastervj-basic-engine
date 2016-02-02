#include "Scene\PresentSceneRendererCommand.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &TreeNode) :CSceneRendererCommand(TreeNode){

}

void CPresentSceneRendererCommand::Execute(CContextManager &_context){
	_context.Present();
}