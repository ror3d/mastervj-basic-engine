#pragma once

#include <Base/Utils/Named.h>
#include <vector>

class CElement;

class CScene : public CNamed
{
	std::vector<CElement*> m_Elements;
public:
	CScene();

	void Load(const std::string& file);
};
