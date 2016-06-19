#include "Mesh.h"

#include "Texture/Texture.h"
#include "Material/Material.h"
#include "Mesh/RenderableVertexs.h"
#include "Renderable/RenderableObjectTechnique.h"


CMesh::CMesh(const std::vector<CRenderableVertexs*> &vtxs,
			 const std::vector<CMaterial *> &mats)
	: m_renderableVertexs(vtxs)
	, m_materials(mats)
{
}

CMesh::CMesh(CXMLTreeNode& node)
	: CNamed(node)
{
}

CMesh::~CMesh()
{
	Destroy();
}

void CMesh::Render(CContextManager *_context)
{
	for (size_t i = 0; i<m_renderableVertexs.size(); ++i)
	{
		CMaterial *l_Material = m_materials[i];
		if (l_Material != NULL && l_Material->getRenderableObjectTechique() != NULL)
		{
			l_Material->apply();
			m_renderableVertexs[i]->RenderIndexed(_context,
				l_Material->getRenderableObjectTechique()->GetEffectTechnique());
		}
	}
}

void CMesh::Destroy()
{
	for (auto it = m_renderableVertexs.begin(); it != m_renderableVertexs.end(); ++it)
	{
		delete *it;
	}
	m_renderableVertexs.clear();
	m_materials.clear();
}
