#include "Scene\EnableAlphaBlendSceneRendererCommand.h"

void CEnableAlphaBlendSceneRendererCommand::Execute(CContextManager &_context){
	_context.EnableAlphaBlendState();
}