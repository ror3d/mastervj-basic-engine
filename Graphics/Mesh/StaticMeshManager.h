#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H

#include <Utils/TMapManager.h>

class CMesh;

class CStaticMeshManager : public TMapManager<CMesh>
{
protected:
	std::string m_FileName;
public:
	CStaticMeshManager();
	~ CStaticMeshManager ();
	bool Load(const std::string &FileName);
	bool Reload();
	std::map<std::string, CMesh*> GetResources(){ return m_resources;  }
};

#endif