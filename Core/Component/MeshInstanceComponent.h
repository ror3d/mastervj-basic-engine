#pragma once
#include "Component.h"

class CMesh;

class CMeshInstanceComponent :
	public CComponent
{
	CMesh *m_StaticMesh;
	std::vector<std::string> m_layers;

public:
	CMeshInstanceComponent(CXMLTreeNode& node, CElement* Owner);
	CMeshInstanceComponent(const CMeshInstanceComponent& base, CElement* Owner);
	~CMeshInstanceComponent();

	void Render(CContextManager&  _context);
	void Destroy();

	inline CMesh * GetStaticMesh() const { return m_StaticMesh; }
	void SetStaticMesh(const std::string& coreName);

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CMeshInstanceComponent( *this, Owner ); }
};

