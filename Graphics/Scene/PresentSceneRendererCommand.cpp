#include "Scene\PresentSceneRendererCommand.h"

void CPresentSceneRendererCommand::Execute(CContextManager &_context){
	_context.Present();
}