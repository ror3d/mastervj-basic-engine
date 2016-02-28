#pragma once

#include <map>
#include <string>
#include "Utils.h"

template<class T>
class TMapManager
{
protected:
	typedef std::map<std::string, T*> resourceMap_t;
	resourceMap_t m_resources;

public:
	virtual ~TMapManager();
	virtual T* get(const std::string& name) const;
	virtual void add(const std::string& name, T* instance);
	virtual void remove(const std::string& name);
	virtual void destroy();
};

template<class T>
T* TMapManager<T>::get( const std::string& name ) const
{
	auto it = m_resources.find( name );
	if ( it != m_resources.end() )
	{
		return it->second;
	}
	return nullptr;
}

template<class T>
void TMapManager<T>::add( const std::string& name, T* instance )
{
	DEBUG_ASSERT( m_resources.find( name ) == m_resources.end() );
	m_resources[name] = instance;
}

template<class T>
void TMapManager<T>::remove( const std::string& name )
{
	DEBUG_ASSERT( m_resources.find( name ) != m_resources.end() );
	m_resources.erase(name);
}

template<class T>
TMapManager<T>::~TMapManager()
{
	DEBUG_ASSERT( m_resources.size() == 0 || "Resources not destroyed!" );
}

template<class T>
void TMapManager<T>::destroy()
{
	for ( auto it = m_resources.begin(); it != m_resources.end(); ++it )
	{
		delete it->second;
	}
	m_resources.clear();
}
