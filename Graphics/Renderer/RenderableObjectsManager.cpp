#include "RenderableObjectsManager.h"
#include "MeshInstance.h"

CRenderableObjectsManager::CRenderableObjectsManager()
{
}


CRenderableObjectsManager::~CRenderableObjectsManager()
{
	destroy();
}


void CRenderableObjectsManager::Update(float ElapsedTime)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Update(ElapsedTime);
	}
}


void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Render(RM);
	}
}


//No se si també s'hauria de llegir Yaw, Pitch, Roll y Visible, però com al constructor de moment no especifica no ho poso.
CRenderableObject * CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &TreeNode)
{
	std::string CoreName = TreeNode.GetPszProperty("core_name");
	std::string InstanceName = TreeNode.GetPszProperty("name");
	Vect3f Default(0.0f, 0.0f, 0.0f);
	Vect3f Position = TreeNode.GetVect3fProperty("pos", Default, false);

	return AddMeshInstance(CoreName, InstanceName, Position);
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


// TODO: Crec que va dir que aquests dos de moment no els fessim
CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(CXMLTreeNode &TreeNode)
{
	return NULL;
}


CRenderableObject * CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position)
{
	return NULL;
}


// TODO: No se si el path del fitxer s'ha de construir o el trobarà bé.
void CRenderableObjectsManager::Load(const std::string &FileName)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
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
			}
		}
	}
}