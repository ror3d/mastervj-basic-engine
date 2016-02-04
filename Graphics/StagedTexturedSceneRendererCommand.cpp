#include "Scene\StagedTexturedSceneRendererCommand.h"

CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(CXMLTreeNode &TreeNode):CSceneRendererCommand(TreeNode){

}
CStagedTexturedSceneRendererCommand::~CStagedTexturedSceneRendererCommand(){}
void CStagedTexturedSceneRendererCommand::CreateRenderTargetViewVector(){
}
void CStagedTexturedSceneRendererCommand::ActivateTextures(){
}
void CStagedTexturedSceneRendererCommand::AddStageTexture(int StageId, CTexture *Texture){
}