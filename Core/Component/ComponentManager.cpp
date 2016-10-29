#include "ComponentManager.h"

#include "Graphics/Context/ContextManager.h"

std::vector<const char*> CComponentManager::s_componentsUpdateOrder =
{
	"Spawn",
	"Collider",
	"Trigger",
	"CharacterController",
	"Camera",
	"FreeCamera",
	"AnimatedInstance",
	"Speaker",
	"ParticleEmitter",
	"MeshInstance",
	"Spawn",
	"Script"
};

CComponentManager::CComponentManager()
	: m_initialized(false)
{
}

CComponentManager::~CComponentManager()
{
}

void CComponentManager::PhysxUpdate()
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->PhysxUpdate();
	}
}

void CComponentManager::FixedUpdate(double ElapsedTime)
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		if ( std::find( m_componentsToRemove.begin(), m_componentsToRemove.end(), *it ) != m_componentsToRemove.end() )
		{
			continue;
		}

		(*it)->FixedUpdate(ElapsedTime);
	}

	DestroyRemovedComponents();
}

void CComponentManager::Update( double ElapsedTime )
{
	for ( auto &const ca : m_componentsToAdd )
	{
		auto it = m_components.find( ca );

		if ( it == m_components.end() )
		{
			m_components.emplace( ca );
			m_componentsByType[ca->GetComponentType()].emplace( ca );
			m_componentsMap[ca->getName()] = ca;
		}
	}
	m_componentsToAdd.clear();

	for ( auto & type : s_componentsUpdateOrder )
	{
		for ( auto cp : m_componentsByType[type] )
		{
			if ( std::find( m_componentsToRemove.begin(), m_componentsToRemove.end(), cp ) != m_componentsToRemove.end() )
			{
				continue;
			}
			if (type == "ParticleEmitter")
			{
				cp->GetEnabled(); 
			}
			
			cp->Update(ElapsedTime);
		}
	}
}

void CComponentManager::Render(CContextManager&  _context)
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		if ( std::find( m_componentsToRemove.begin(), m_componentsToRemove.end(), *it ) != m_componentsToRemove.end() )
		{
			continue;
		}

		(*it)->Render(_context);
	}
}
void CComponentManager::RenderDebug(CContextManager&  _context)
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		if ( std::find( m_componentsToRemove.begin(), m_componentsToRemove.end(), *it ) != m_componentsToRemove.end() )
		{
			continue;
		}

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
			m_componentsByType[cr->GetComponentType()].erase( cr );
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
