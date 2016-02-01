#include "Scene\DisableAlphaBlendSceneRendererCommand.h"

void CDisableAlphaBlendSceneRendererCommand::Execute(CContextManager &_context){
	_context.DisableAlphaBlendState();
}