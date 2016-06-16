#pragma once
#include <Base/Utils/TMapManager.h>

class CScene;
class CSceneManager : public TMapManager<CScene>
{
public:
	CSceneManager();
	virtual ~CSceneManager();

	void Initialize( std::string scenesDirectory );

	void EnableScene( const std::string& scene );
	void DisableScene( const std::string& scene );
};
