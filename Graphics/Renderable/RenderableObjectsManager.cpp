#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "Mesh/MeshInstance.h"
#include "Renderable/AnimatedInstanceModel.h"

CRenderableObjectsManager::CRenderableObjectsManager()
{
}


CRenderableObjectsManager::~CRenderableObjectsManager()
{
}


void CRenderableObjectsManager::Update(float ElapsedTime)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Update(ElapsedTime);
	}
}


void CRenderableObjectsManager::Render(CContextManager *_context)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Render(_context);
	}
}


CRenderableObject * CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &TreeNode)
{
	std::string InstanceName = TreeNode.GetPszProperty("name");
	auto it = m_resources.find(InstanceName);

	if (it != m_resources.end())
	{
		return it->second;
	}
	else
	{
		std::string CoreName = TreeNode.GetPszProperty("core_name");
		Vect3f Default(0.0f, 0.0f, 0.0f);

		CMeshInstance * Object = new CMeshInstance(InstanceName, CoreName);
		Object->SetPosition(TreeNode.GetVect3fProperty("pos", Default, false));
		Object->SetYaw(TreeNode.GetFloatProperty("yaw"));
		Object->SetPitch(TreeNode.GetFloatProperty("pitch"));
		Object->SetRoll(TreeNode.GetFloatProperty("roll"));

		add(InstanceName, Object);
		return Object;
	}
}


CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
	auto it = m_resources.find(InstanceName);

	if (it != m_resources.end())
	{
		return it->second;
	}
	else
	{
		CMeshInstance * Object = new CMeshInstance(InstanceName, CoreMeshName);
		Object->SetPosition(Position);
		add(InstanceName, Object);
		return Object;
	}
}


CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(CXMLTreeNode &TreeNode)
{
	std::string InstanceName = TreeNode.GetPszProperty("name");
	auto it = m_resources.find(InstanceName);

	if (it != m_resources.end())
	{
		return it->second;
	}
	else
	{
		std::string CoreName = TreeNode.GetPszProperty("core_name");
		Vect3f Default(0.0f, 0.0f, 0.0f);

		CAnimatedInstanceModel * Object = new CAnimatedInstanceModel(TreeNode);
		Object->SetPosition(TreeNode.GetVect3fProperty("pos", Default, false));
		Object->SetYaw(TreeNode.GetFloatProperty("yaw"));
		Object->SetPitch(TreeNode.GetFloatProperty("pitch"));
		Object->SetRoll(TreeNode.GetFloatProperty("roll"));

		add(InstanceName, Object);
		return Object;
	}
}


CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
	return NULL;
}

