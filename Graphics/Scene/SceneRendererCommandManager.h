#ifndef SCENE_RENDERER_COMMAND_MANAGER_H
#define SCENE_RENDERER_COMMAND_MANAGER_H

#include "Utils\TMapManager.h"
#include "Scene\SceneRendererCommand.h"

class CSceneRendererCommandManager : public
	TMapManager<CSceneRendererCommand>
{
private:
	std::string m_Filename;
public:
	CSceneRendererCommandManager();
	virtual ~CSceneRendererCommandManager();
	std::string GetNextName();
	bool Load(const std::string &Filename);
	bool Reload();
	void Execute(CContextManager &_context);
};

#endif