#pragma once

#include <vector>
#include <string>

#include <Base/Utils/Named.h>
#include <Base/Math/MathTypes.h>
#include <Graphics/Renderable/Renderable.h>
#include "VertexTypes.h"

class CRenderableVertexs;
class CMaterial;
class CXMLTreeNode;

class CMesh : public CNamed, public IRenderable
{
protected:
	std::vector<CRenderableVertexs*> m_renderableVertexs;
	std::vector<CMaterial *> m_materials;
	CMesh() {}
	CMesh(CXMLTreeNode& node);
public:
	CMesh(const std::vector<CRenderableVertexs*> &vtxs, const std::vector<CMaterial *> &mats);
	~CMesh();

	inline void setMaterial(unsigned int idx, CMaterial* mat)
	{
		DEBUG_ASSERT(idx < m_materials.size());
		m_materials[idx] = mat;
	}

	void Render (CContextManager *_context);

	void Destroy();
};

