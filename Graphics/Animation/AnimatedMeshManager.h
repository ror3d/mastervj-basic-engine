#pragma once

#include <Utils/TMapManager.h>

class CAnimatedMesh;

class CAnimatedMeshManager : public TMapManager<CAnimatedMesh>
{
private:
	std::string m_Filename;
public:
	CAnimatedMeshManager();
	virtual ~CAnimatedMeshManager();
	void Load(const std::string &Filename);
	void Reload();
};
