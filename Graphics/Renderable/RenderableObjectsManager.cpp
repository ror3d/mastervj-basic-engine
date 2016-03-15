#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "Mesh/MeshInstance.h"
#include "Mesh/StaticMesh.h"
#include "Animation/AnimatedInstanceModel.h"
#include "Particles/ParticleSystemInstance.h"

CRenderableObjectsManager::CRenderableObjectsManager()
{
}


CRenderableObjectsManager::~CRenderableObjectsManager()
{
	destroy();
}


void CRenderableObjectsManager::Update(float ElapsedTime)
{
	if (m_active){
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			it->second->Update(ElapsedTime);
		}
	}
}


void CRenderableObjectsManager::Render(CContextManager *_context)
{
	if (m_active)
	{
		CEffectManager *l_effectManager = CEngine::GetSingleton().getEffectsManager();

		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			l_effectManager->m_SceneParameters.m_World = it->second->GetTransform();
			it->second->Render(_context);
		}
	}
}


void CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &TreeNode)
{
	std::string InstanceName = TreeNode.GetPszProperty("name");
	auto it = m_resources.find(InstanceName);

	if (it != m_resources.end())
	{
		return;
	}
	else
	{
		CMeshInstance * Object = new CMeshInstance(TreeNode);
		add(InstanceName, Object);
		return;
	}
}


void CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
	auto it = m_resources.find(InstanceName);

	if (it != m_resources.end())
	{
		return;
	}
	else
	{
		CMeshInstance * Object = new CMeshInstance(InstanceName, CoreMeshName);
		Object->SetPosition(Position);
		add(InstanceName, Object);
	}
}


void CRenderableObjectsManager::AddAnimatedInstanceModel(CXMLTreeNode &TreeNode)
{
	std::string InstanceName = TreeNode.GetPszProperty("name");
	auto it = m_resources.find(InstanceName);

	if (it != m_resources.end())
	{
		return;
	}
	else
	{
		CAnimatedInstanceModel * Object = new CAnimatedInstanceModel(TreeNode);

		add(InstanceName, Object);
	}
}


void CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
	// TODO!
	DEBUG_ASSERT(!"NOT IMPLEMENTED");
}

void CRenderableObjectsManager::AddParticleEmitter(CXMLTreeNode &node)
{
	auto pe = new CParticleSystemInstance(node);

	add(pe->getName(), pe);
}
