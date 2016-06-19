#pragma once
#include <Base/Utils/TMapManager.h>

class CScene;
class CElement;

class CSceneManager : public TMapManager<CScene>
{
	std::map<std::string, CElement*> m_Objects;

public:
	CSceneManager();
	virtual ~CSceneManager();

	void Initialize( std::string scenesDirectory );

	void EnableScene( const std::string& scene );
	void DisableScene( const std::string& scene );


	void AddObject( CElement* obj );
	void DestroyObject(const std::string& id);

	CElement* GetObjectById( const std::string& id );
};
