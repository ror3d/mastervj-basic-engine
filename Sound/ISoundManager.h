#pragma once

#include <string>

class ISoundManager
{
public:
	static ISoundManager* InstantiateSoundManager();

	virtual ~ISoundManager() {}

	void SetPath(const std::string &path) { m_Path = path; }

protected:
	ISoundManager()
	{}

//protected:
public:
	std::string		m_Path;
};