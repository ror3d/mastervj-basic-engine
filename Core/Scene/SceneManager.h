#pragma once
#include <Base/Utils/TMapManager.h>
#include <vector>

class CScene;
class CElement;

class CSceneManager : public TMapManager<CScene>
{
	std::map<std::string, CElement*> m_Objects;

	std::vector<std::string> m_ScenesToLoad;
	std::vector<std::string> m_ScenesToUnload;
public:
	CSceneManager();
	virtual ~CSceneManager();

	void Initialize( std::string scenesDirectory );

	void Update();

	void LoadScene( const std::string& sceneName );
	void UnloadScene( const std::string& sceneName );


	void AddObject( CElement* obj );
	void DestroyObject(const std::string& id);

	CElement* GetObjectById( const std::string& id );
};
