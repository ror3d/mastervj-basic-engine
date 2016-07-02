#include "ComponentManager.h"

#include "Graphics/Context/ContextManager.h"

CComponentManager::CComponentManager()
	: m_initialized(false)
{
}

CComponentManager::~CComponentManager()
{
}

void CComponentManager::FixedUpdate(float ElapsedTime)
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->FixedUpdate(ElapsedTime);
	}
}

void CComponentManager::Update(float ElapsedTime)
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->Update(ElapsedTime);
	}
}
void CComponentManager::Render(CContextManager&  _context)
{
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
		m_components.emplace(Component);
		m_componentsMap[Component->getName()] = Component;
		return true;
	}

	return false;
}

bool CComponentManager::RemoveComponent(CComponent* Component)
{
	auto it = m_components.find(Component);

	if (it != m_components.end())
	{
		m_componentsMap.erase(Component->getName());
		m_components.erase(it);
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
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->Destroy();
	}
	m_components.clear();
	m_componentsMap.clear();
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
