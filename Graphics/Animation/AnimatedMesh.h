#pragma once
#include "Mesh/Mesh.h"

class CalModel;
class CalHardwareModel;
class CalCoreModel;
class CXMLTreeNode;
class CAnimatedMeshInstance;

class CAnimatedMesh :
	public CMesh
{
	CalCoreModel *m_CalCoreModel;
	CalHardwareModel *m_CalHardwareModel;

	void Init();
	bool LoadVertexBuffers();

	void Render(CContextManager* ctx) { CMesh::Render(ctx); }
public:
	CAnimatedMesh(CXMLTreeNode &node);
	~CAnimatedMesh();

	void Render(CContextManager* context, CalModel *calModel);

	inline CalCoreModel *GetCoreModel() const { return m_CalCoreModel; }
};

