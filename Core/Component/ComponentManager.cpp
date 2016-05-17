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
	}
}

void CComponentManager::destroy()
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		(*it)->Destroy();
	}
}
