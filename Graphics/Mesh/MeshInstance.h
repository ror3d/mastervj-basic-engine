#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "Renderable/RenderableObject.h"

class CStaticMesh;
class CContextManager;

class CMeshInstance : public CRenderableObject
{
private:
	CStaticMesh *m_StaticMesh;
public:
	CMeshInstance(const std::string &Name, const std::string &CoreName);
	~CMeshInstance();
	void Render(CContextManager *_context);
};

#endif