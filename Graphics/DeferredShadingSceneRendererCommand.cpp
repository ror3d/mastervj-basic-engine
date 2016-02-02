#include "Scene/DeferredShadingSceneRendererCommand.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode) 
	: CStagedTexturedSceneRendererCommand(TreeNode){

}

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand(){
}

void CDeferredShadingSceneRendererCommand::Execute(CContextManager &_context){

}