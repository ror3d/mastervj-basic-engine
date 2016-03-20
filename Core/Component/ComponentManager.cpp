#include "ComponentManager.h"

#include "Graphics/Context/ContextManager.h"

CComponentManager::CComponentManager()
{
}

CComponentManager::~CComponentManager()
{
}

void CComponentManager::Update(float ElapsedTime)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); it++)
	{
		it->second->Update(ElapsedTime);
	}
}
void CComponentManager::Render(CContextManager  &_context)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); it++)
	{
		it->second->Render(_context);
	}
}
void CComponentManager::RenderDebug(CContextManager  &_context)
{	
	for (auto it = m_resources.begin(); it != m_resources.end(); it++)
	{
		it->second->RenderDebug(_context);
	}
}

bool CComponentManager::AddComponent(const std::string &Name, CComponent *Component)
{	
	auto it = m_resources.find(Name);
	if (it == m_resources.end()){
		add(Name, Component);
	}
	return false;
}
