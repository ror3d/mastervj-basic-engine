#pragma once

#include <map>
#include <set>
#include <string>
#include "Utils.h"
#include "Named.h"

struct BoolRefCount_t
{
	bool b;
	unsigned int c;

	BoolRefCount_t()
		: b(false)
		, c(0)
	{}

	BoolRefCount_t(const BoolRefCount_t &r)
		: b(r.b)
		, c(r.c)
	{}


	BoolRefCount_t(BoolRefCount_t&& r)
		: b(r.b)
		, c(r.c)
	{
		r.b = false;
		r.c = 0;
	}

};

template<class T>
class TMapManager
{
public:
	typedef std::map<std::string, T*> resourceMap_t;
	typedef typename resourceMap_t::iterator iterator;
	typedef typename resourceMap_t::const_iterator const_iterator;

	typedef std::set<BoolRefCount_t*> dirtyReferencesSet_t;
	
protected:
	resourceMap_t m_resources;
	dirtyReferencesSet_t m_dirtyReferences;

	T* get_internal(const std::string& name) const;
	bool m_reloading;

public:
	typedef TMapManager<T> MapManagerType_t;

	class Ref
	{
	private:
		std::string name;
		T* ref;
		BoolRefCount_t* dirty;
		TMapManager<T> * manager;

		Ref(const Ref& r) = delete;
		void operator=(const Ref& r) = delete;
	public:

		Ref()
			: name()
			, ref(nullptr)
			, dirty(nullptr)
			, manager(nullptr)
		{ }

		Ref(const std::string& name, T* ref, BoolRefCount_t* dirty, TMapManager<T> * manager)
			: name(name)
			, ref(ref)
			, dirty(dirty)
			, manager(manager)
		{
			dirty->c++;
		}

		Ref(Ref&& r)
			: name(std::move(r.name))
			, ref(r.ref)
			, dirty(r.dirty)
			, manager(r.manager)
		{
			r.ref = nullptr;
			r.dirty = nullptr;
			r.manager = nullptr;
		}

		~Ref()
		{
			if (dirty != nullptr)
			{
				manager->release(dirty);
				dirty = nullptr;
			}
			manager = nullptr;
			ref = nullptr;
		}


		void operator=(Ref&& r)
		{
			name = std::move(r.name);
			ref = r.ref;
			dirty = r.dirty;
			manager = r.manager;
			r.ref = nullptr;
			r.dirty = nullptr;
			r.manager = nullptr;
		}

		T* operator-> ()
		{
			if (dirty->b)
			{
				ref = manager->get_internal(name);
				DEBUG_ASSERT(ref);
				dirty->b = false;
			}
			return ref;
		}

		T& operator* ()
		{
			if (dirty->b)
			{
				ref = manager->get_internal(name);
				DEBUG_ASSERT(ref);
				dirty->b = false;
			}
			return *ref;
		}

		bool operator==(T* r) const
		{
			return r == ref;
		}

		bool operator!=(T* r) const
		{
			return r != ref;
		}

		bool operator!() const
		{
			return !ref;
		}

		operator bool() const
		{
			return !!ref;
		}

		T* getRef()
		{
			if (dirty != NULL)
			{
				if (dirty->b)
				{
					ref = manager->get_internal(name);
					DEBUG_ASSERT(ref);
					dirty->b = false;
				}
			}
			return ref;
		}

		void setRef(const std::string& refName)
		{
			name = refName;
			dirty->b = true;
		}
	};

	virtual ~TMapManager();

	virtual Ref ref(const std::string& name)
	{
		BoolRefCount_t* dirty = new BoolRefCount_t();
		m_dirtyReferences.insert(dirty);
		T* ref = get_internal(name);
		return Ref(name, ref, dirty, this);
	}

	virtual T* get(const std::string& name) const;
	virtual void add(const std::string& name, T* instance);
	template < typename = typename std::enable_if< std::is_base_of<CNamed, T>::value >::type >
		void add(T* instance);
	virtual void remove(const std::string& name);
	virtual void release(BoolRefCount_t* dirty);
	virtual void setDirty();
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
	//DEBUG_ASSERT( m_resources.find( name ) == m_resources.end() );
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

template<class T>
void TMapManager<T>::setDirty()
{
	for (auto d : m_dirtyReferences)
	{
		d->b = true;
	}
}

template<class T>
void TMapManager<T>::release(BoolRefCount_t* dirty)
{
	dirty->c--;
	if (!dirty->c)
	{
		m_dirtyReferences.erase(dirty);
		delete dirty;
	}
}

template<class T>
T* TMapManager<T>::get_internal(const std::string& name) const
{
	auto it = m_resources.find(name);
	if (it != m_resources.end())
	{
		return it->second;
	}
	return nullptr;
}