#include "StaticMeshManager.h"


CStaticMeshManager::CStaticMeshManager()
	: m_FileName("")
{
}

CStaticMeshManager::~ CStaticMeshManager ()
{
	destroy();
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
		for (int i = 0; i < l_StaticMesh.GetNumChildren(); ++i){
			CXMLTreeNode l_Mesh = l_StaticMesh(i);

			if (l_Mesh.GetName() == std::string("static_mesh"))
			{

				std::string m_Name = l_Mesh.GetPszProperty("name");
				std::string l_Path = l_Mesh.GetPszProperty("filename", "");

				CStaticMesh* l_static_mesh = new CStaticMesh();

				if(l_static_mesh->Load(l_Path))
				{
						add(m_Name, l_static_mesh);
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
