#ifndef COOKED_MESH_MANAGER_H
#define COOKED_MESH_MANAGER_H

#include <Base/Utils/TMapManager.h>
#include <Base/Math/MathTypes.h>

#include <vector>

class CCookedMeshManager : TMapManager<std::vector<uint8>>
{
protected:
	std::string m_cookedMeshesPath;

	std::string GetValidName( const std::string path );
public:
	CCookedMeshManager();
	~CCookedMeshManager();

	bool Load( const std::string& meshName, std::vector<uint8> * &cooked );

	void add(const std::string& meshName, std::vector<uint8>* cooked);

	void SetCookedMeshPath( const std::string& path ) { m_cookedMeshesPath = path; }
};

#endif
