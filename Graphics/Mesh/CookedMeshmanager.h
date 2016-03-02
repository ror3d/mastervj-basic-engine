#ifndef COOKED_MESH_MANAGER_H
#define COOKED_MESH_MANAGER_H

class CCookedMeshManager 
{
protected:
public:
	CCookedMeshManager();
	~CCookedMeshManager();
	
	void CookMeshes();
	void CreateActors();
};

#endif