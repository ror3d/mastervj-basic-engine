#pragma once
#include "Component.h"

class CStaticMesh;

class CMeshInstanceComponent :
	public CComponent
{
	CStaticMesh *m_StaticMesh;
public:
	CMeshInstanceComponent(const std::string& name, CElement* Owner);
	CMeshInstanceComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	~CMeshInstanceComponent();

	void Render(CContextManager&  _context);
	void Destroy();

	inline CStaticMesh * GetStaticMesh() const { return m_StaticMesh; }
	void SetStaticMesh(const std::string& coreName);
};

