#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "Mesh/MeshInstance.h"
#include "Mesh/StaticMesh.h"
#include "Renderable/AnimatedInstanceModel.h"

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
	if (m_active){
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			CEffectManager l_effectManager = *(CEngine::GetSingleton().getEffectsManager());

			l_effectManager.m_SceneParameters.m_World = it->second->GetTransform();
			l_effectManager.SetSceneConstants();

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
		CPhysxColliderShapeDesc desc;
		desc.shape = CPhysxColliderShapeDesc::Shape::Box;
		desc.density = 1;
		desc.material = "box";
		Vect3f sizeColision = Vect3f(0,0,0);
		sizeColision.x = Object->getStaticMesh()->GetAabbMax().x - Object->getStaticMesh()->GetAabbMin().x;
		sizeColision.y = Object->getStaticMesh()->GetAabbMax().y - Object->getStaticMesh()->GetAabbMin().y;
		sizeColision.z = Object->getStaticMesh()->GetAabbMax().z - Object->getStaticMesh()->GetAabbMin().z;
		desc.size = sizeColision;
		desc.position = Object->GetPosition() + Object->getStaticMesh()->GetBsCenter();
		//CEngine::GetSingleton().getPhysicsManager()->createActor(Object->getStaticMesh()->getName(), CPhysXManager::ActorType::Static, desc);
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
}


void CRenderableObjectsManager::Load(const std::string &FileName)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		m_FileName = FileName;

		CXMLTreeNode l_Meshes = l_XML["renderable_objects"];
		if (l_Meshes.Exists())
		{
			for (int i = 0; i < l_Meshes.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Mesh = l_Meshes(i);

				if (l_Mesh.GetName() == std::string("mesh_instance"))
				{
					AddMeshInstance(l_Mesh);
				}
				else if (l_Mesh.GetName() == std::string("animated_instance"))
				{
					AddAnimatedInstanceModel(l_Mesh);
				}
			}
		}
	}
}

void CRenderableObjectsManager::reload()
{
	destroy();
	Load(m_FileName);
}