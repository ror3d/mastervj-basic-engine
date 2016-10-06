#pragma once

#include <map>
#include <string>
#include "Utils.h"
#include "Named.h"

template<class T>
class TMapContainer
{
public:
	typedef std::map<std::string, T*> resourceMap_t;
	typedef typename resourceMap_t::iterator iterator;
	typedef typename resourceMap_t::const_iterator const_iterator;
protected:
	resourceMap_t m_resources;

public:
	typedef TMapContainer<T> MapManagerType_t;

	virtual ~TMapContainer();
	virtual T* get(const std::string& name) const;
	virtual void add(const std::string& name, T* instance);
	template < typename = typename std::enable_if< std::is_base_of<CNamed, T>::value >::type >
		void add(T* instance);
	virtual void remove(const std::string& name);
	virtual void clear();
	virtual void destroy();

	inline size_t size() const { return m_resources.size(); }

	iterator begin() { return m_resources.begin(); }
	iterator end() { return m_resources.end(); }
	const_iterator begin() const { return m_resources.begin(); }
	const_iterator end() const { return m_resources.end(); }
};

template<class T>
T* TMapContainer<T>::get( const std::string& name ) const
{
	auto it = m_resources.find( name );
	if ( it != m_resources.end() )
	{
		return it->second;
	}
	return nullptr;
}

template<class T>
void TMapContainer<T>::add( const std::string& name, T* instance )
{
	//DEBUG_ASSERT( m_resources.find( name ) == m_resources.end() );
	m_resources[name] = instance;
}

template <class T>
	template< typename = typename std::enable_if< std::is_base_of<CNamed, T>::value >::type >
void TMapContainer<T>::add(T* instance)
{
	add(instance->getName(), instance);
}

template<class T>
void TMapContainer<T>::remove( const std::string& name )
{
	DEBUG_ASSERT( m_resources.find( name ) != m_resources.end() );
	m_resources.erase(name);
}

template<class T>
TMapContainer<T>::~TMapContainer()
{
	DEBUG_ASSERT( m_resources.size() == 0 || "Resources not destroyed!" );
}

template<class T>
void TMapContainer<T>::clear()
{
	m_resources.clear();
}

template<class T>
void TMapContainer<T>::destroy()
{
	for ( auto it = m_resources.begin(); it != m_resources.end(); ++it )
	{
		delete it->second;
	}
	m_resources.clear();
}
