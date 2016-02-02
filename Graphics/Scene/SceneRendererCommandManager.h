#ifndef SCENE_RENDERER_COMMAND_MANAGER_H
#define SCENE_RENDERER_COMMAND_MANAGER_H

#include <vector>
#include <assert.h>
class CSceneRendererCommand;
class CSceneRendererCommandManager
{
private:
	std::string m_Filename;
	std::vector<CSceneRendererCommand *> m_resources;
	
public:
	CSceneRendererCommandManager();
	virtual ~CSceneRendererCommandManager();
	std::string GetNextName();
	bool Load(const std::string &Filename);
	bool Reload();
	void Execute(CContextManager &_context);
	void add(CSceneRendererCommand * command);
	void destroy();
};

#endif