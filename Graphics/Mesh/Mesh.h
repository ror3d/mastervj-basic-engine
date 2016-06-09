#pragma once

#include <vector>
#include <string>

#include <Base/Utils/Named.h>
#include <Base/Math/MathTypes.h>
#include "VertexTypes.h"

class CRenderableVertexs;
class CMaterial;
class CXMLTreeNode;

class CMesh
{
protected:
	std::vector<CRenderableVertexs*> m_renderableVertexs;
	std::vector<CMaterial *> m_materials;
public:
	CMesh();
	CMesh(const std::vector<CRenderableVertexs*> &vtxs, const std::vector<CMaterial *> &mats);
	~CMesh();

	inline void setMaterial(unsigned int idx, CMaterial* mat)
	{
		DEBUG_ASSERT(idx < m_materials.size());
		m_materials[idx] = mat;
	}
};

