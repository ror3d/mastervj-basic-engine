#pragma once
#include "Component.h"

class CMesh;

class CMeshInstanceComponent :
	public CComponent
{
	CMesh *m_StaticMesh;
	std::vector<std::string> m_layers;

public:
	CMeshInstanceComponent(const std::string& name, CElement* Owner);
	CMeshInstanceComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	~CMeshInstanceComponent();

	void Render(CContextManager&  _context);
	void Destroy();

	inline CMesh * GetStaticMesh() const { return m_StaticMesh; }
	void SetStaticMesh(const std::string& coreName);
};

