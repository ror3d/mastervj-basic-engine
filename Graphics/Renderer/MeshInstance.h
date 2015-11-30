#pragma once
#include "RenderableObject.h"
#include "StaticMesh.h"
#include "RenderManager.h"
//#include "Core/Engine.h>
#include <Utils/Named.h>

class CMeshInstance : public CRenderableObject
{
private:
	CStaticMesh *m_StaticMesh;
public:
	CMeshInstance(const std::string &Name, const std::string &CoreName);
	~CMeshInstance();
	void Render(CRenderManager *RM);
};