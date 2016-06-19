#pragma once

#include <Base/Utils/Named.h>
#include <vector>

class CSceneManager;

class CScene : public CNamed
{
	std::vector<std::string> m_Elements;
	CSceneManager* m_sm;
public:
	CScene();
	virtual ~CScene();

	void Load( const std::string& file, CSceneManager* manager );
};
