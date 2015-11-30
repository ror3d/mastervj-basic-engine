#pragma once
#include <vector>
#include <string>

#include "Utils/Named.h"
#include "VertexTypes.h"

class CRenderManager;
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

public:
	CStaticMesh();
	virtual ~CStaticMesh();

	bool Load (const std::string &FileName);
	bool Reload ();
	void Render (CRenderManager *RM) const;
	void Destroy();
};     
