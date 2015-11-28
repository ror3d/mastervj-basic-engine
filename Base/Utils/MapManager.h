#pragma once

#include <map>
#include <string>

template<class T>
class TemplatedMapManager
{
protected:
	typedef std::map<std::string, T*> resourceMap_t;
	resourceMap_t m_resources;

public:
	virtual T* get(const std::string& name);
	virtual void add(const std::string& name, T* instance);
	void destroy();
};
