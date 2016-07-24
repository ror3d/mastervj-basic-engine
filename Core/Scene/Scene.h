#pragma once

#include <Base/Utils/Named.h>
#include <vector>

class CSceneManager;

class CScene : public CNamed
{
	std::vector<std::string> m_Elements;
	CSceneManager* m_sm;
	std::string m_SceneFile;
	bool m_isLoaded;
public:
	CScene( const std::string& file, CSceneManager* manager );
	virtual ~CScene();

	void Load();
	void Unload();

	void AddObject( const std::string& objName );
	void RemoveObject( const std::string& objName );
};
