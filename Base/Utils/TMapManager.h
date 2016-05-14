#pragma once

#include <map>
#include <string>
#include "Utils.h"
#include "Named.h"

template<class T>
class TMapManager
{
public:
	typedef std::map<std::string, T*> resourceMap_t;
	typedef typename resourceMap_t::iterator iterator;
	typedef typename resourceMap_t::const_iterator const_iterator;
protected:
	resourceMap_t m_resources;

public:
	typedef TMapManager<T> MapManagerType_t;

	virtual ~TMapManager();
	virtual T* get(const std::string& name) const;
	virtual void add(const std::string& name, T* instance);
	template < typename = typename std::enable_if< std::is_base_of<CNamed, T>::value >::type >
		void add(T* instance);
	virtual void remove(const std::string& name);
	virtual void destroy();

	iterator begin() { return m_resources.begin(); }
	iterator end() { return m_resources.end(); }
	const_iterator begin() const { return m_resources.begin(); }
	const_iterator end() const { return m_resources.end(); }
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

template <class T>
	template< typename = typename std::enable_if< std::is_base_of<CNamed, T>::value >::type >
void TMapManager<T>::add(T* instance)
{
	add(instance->getName(), instance);
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
