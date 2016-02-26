#pragma once
#include <vector>
#include <string>

#include <Base/Utils/Named.h>
#include <Base/Math/MathTypes.h>
#include "VertexTypes.h"

class CRenderManager;
class CContextManager;
class CRenderableVertexs;
class CMaterial;

class CStaticMesh : public CNamed
{
protected:
	Vect3f m_BsCenter, m_AabbMin, m_AabbMax;
	float m_BsRadius;
	std::vector<CRenderableVertexs*> m_renderableVertexs;
	std::vector<CMaterial *> m_materials;
	std::string m_fileName;
	void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);

public:
	CStaticMesh();
	virtual ~CStaticMesh();

	bool Load (const std::string &FileName);
	bool Reload ();
	void Render (CContextManager *_context) const;
	void Destroy();

	inline void setMaterial(unsigned int idx, CMaterial* mat)
	{
		DEBUG_ASSERT(idx < m_materials.size());
		m_materials[idx] = mat;
	}
};
