#pragma once
#include <Base/Utils/TMapManager.h>
#include <vector>

class CScene;
class CElement;

class CSceneManager : public TMapManager<CScene>
{
	std::map<std::string, CElement*> m_Objects;

	std::vector<std::string> m_ObjectsToDestroy;

	std::vector<std::string> m_ScenesToLoad;
	std::vector<std::string> m_ScenesToUnload;
public:
	CSceneManager();
	virtual ~CSceneManager();

	void Initialize( std::string scenesDirectory );

	void FixedUpdate();
	void Update();

	void LoadScene( const std::string& sceneName );
	void UnloadScene( const std::string& sceneName );

	void StartedUnload();
	void FinishedLoad();

	void AddObjectToScene( const std::string& sceneName, CElement* obj );
	void DestroyObjectFromScene( const std::string& sceneName, const std::string& objectName );

	CElement* GetObjectById( const std::string& id );

private:
	friend class CScene;

	void AddObject( CElement* obj );
	void DestroyObject(const std::string& id);

};
