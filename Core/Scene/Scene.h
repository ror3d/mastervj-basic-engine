#pragma once

#include <Base/Utils/Named.h>

class CScene : public CNamed
{
public:
	CScene();

	void Load(const std::string& file);
};
