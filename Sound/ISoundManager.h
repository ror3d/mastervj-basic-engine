#pragma once

#include <string>

class ISoundManager
{
public:
	static ISoundManager* InstantiateSoundManager();

	virtual ~ISoundManager() {}

protected:
	ISoundManager()
	{}

protected:
	std::string		m_Path;
};