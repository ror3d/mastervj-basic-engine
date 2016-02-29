#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H

#include <Utils/TMapManager.h>

class CStaticMesh;

class CStaticMeshManager : public TMapManager<CStaticMesh>
{
protected:
	std::string m_FileName;
public:
	CStaticMeshManager();
	~ CStaticMeshManager ();
	bool Load(const std::string &FileName);
	bool Reload();
	std::map<std::string, CStaticMesh*> GetResources(){ return m_resources;  }
};

#endif