#include "StaticMeshManager.h"
#include "StaticMesh.h"
#include <XML/XMLTreeNode.h>

#include <Core/Engine/Engine.h>


CStaticMeshManager::CStaticMeshManager()
	: m_FileName("")
{
}

CStaticMeshManager::~ CStaticMeshManager ()
{
}

bool CStaticMeshManager::Load(const std::string &FileName)
{
	 m_FileName = FileName;
	 CXMLTreeNode l_StaticMesh;

	 l_StaticMesh.LoadFile(FileName.c_str());

	if( !l_StaticMesh.Exists() )
	{
		return false;
	}
	else
	{
		CMaterialManager* mm = CEngine::GetSingleton().getMaterialManager();

		for (int i = 0; i < l_StaticMesh.GetNumChildren(); ++i){
			CXMLTreeNode l_Mesh = l_StaticMesh(i);

			if (l_Mesh.GetName() == std::string("static_mesh"))
			{
				CStaticMesh* l_static_mesh = new CStaticMesh(l_Mesh);

				if(l_static_mesh->Load())
				{
					int nmat = 0;
					for (int j = 0; j < l_Mesh.GetNumChildren(); ++j)
					{
						CXMLTreeNode l_Mat = l_Mesh(j);
						if (l_Mat.GetName() == std::string("material")
							&& l_Mat.GetPszProperty("name") != nullptr)
						{
							CMaterial* mat = mm->get(l_Mat.GetPszProperty("name"));
							l_static_mesh->setMaterial(nmat, mat);
							nmat++;
						}
					}
					add(l_static_mesh->getName(), l_static_mesh);
				}
			}
		}
	}
	return true;
}

bool CStaticMeshManager::Reload()
{
	destroy();
	return Load(m_FileName);
}
