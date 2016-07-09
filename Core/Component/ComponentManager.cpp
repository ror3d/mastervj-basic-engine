#include "ComponentManager.h"

#include "Graphics/Context/ContextManager.h"

CComponentManager::CComponentManager()
	: m_initialized(false)
{
}

CComponentManager::~CComponentManager()
{
}

void CComponentManager::PhysxUpdate()
{
	DestroyRemovedComponents();

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->PhysxUpdate();
	}
}

void CComponentManager::FixedUpdate(float ElapsedTime)
{
	DestroyRemovedComponents();

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->FixedUpdate(ElapsedTime);
	}
}

void CComponentManager::Update( float ElapsedTime )
{
	DestroyRemovedComponents();

	for ( auto &const ca : m_componentsToAdd )
	{
		auto it = m_components.find( ca );

		if ( it == m_components.end() )
		{
			m_components.emplace( ca );
			m_componentsMap[ca->getName()] = ca;
		}
	}
	m_componentsToAdd.clear();

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		if ( std::find( m_componentsToRemove.begin(), m_componentsToRemove.end(), *it ) != m_componentsToRemove.end() )
		{
			continue;
		}

		(*it)->Update(ElapsedTime);
	}

	DestroyRemovedComponents();
}

void CComponentManager::Render(CContextManager&  _context)
{
	DestroyRemovedComponents();

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->Render(_context);
	}
}
void CComponentManager::RenderDebug(CContextManager&  _context)
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->RenderDebug(_context);
	}
}

bool CComponentManager::AddComponent(CComponent* Component)
{
	auto it = m_components.find(Component);

	if (it == m_components.end())
	{
		m_componentsToAdd.push_back( Component );
		return true;
	}

	return false;
}

bool CComponentManager::RemoveComponent(CComponent* Component)
{
	auto it = m_components.find(Component);

	if (it != m_components.end())
	{
		m_componentsToRemove.push_back( Component );
		return true;
	}

	return false;
}

void CComponentManager::FirstInitialization()
{
	if ( !m_initialized )
	{
		m_initialized = true;

		for (auto it = m_components.begin(); it != m_components.end(); it++)
		{
			(*it)->Initialize();
		}

		for (auto it = m_components.begin(); it != m_components.end(); it++)
		{
			(*it)->ObjectInitialized();
		}
	}
}

void CComponentManager::destroy()
{
	for ( auto &const cr : m_componentsToRemove )
	{
		auto it = m_components.find(cr);

		if (it != m_components.end())
		{
			m_componentsMap.erase(cr->getName());
			m_components.erase(it);
			delete cr;
		}
	}
	m_componentsToRemove.clear();

	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->Destroy();
		delete *it;
	}
	m_components.clear();
	m_componentsMap.clear();

	for ( auto &const ca : m_componentsToAdd )
	{
		delete ca;
	}
	m_componentsToAdd.clear();
}

void CComponentManager::DestroyRemovedComponents()
{
	for ( auto &const cr : m_componentsToRemove )
	{
		auto it = m_components.find(cr);

		if (it != m_components.end())
		{
			m_componentsMap.erase(cr->getName());
			m_components.erase(it);
			delete cr;
		}
	}
	m_componentsToRemove.clear();
}


CComponent* CComponentManager::get(const std::string& componentName) const
{
	auto it = m_componentsMap.find(componentName);
	if (it == m_componentsMap.end())
	{
		return nullptr;
	}
	return it->second;
}
