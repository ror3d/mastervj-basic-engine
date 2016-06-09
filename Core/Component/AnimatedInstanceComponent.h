#pragma once
#include "Component.h"
class CAnimatedInstanceComponent :
	public CComponent
{
public:
	CAnimatedInstanceComponent(const std::string& name, CElement* Owner);
	CAnimatedInstanceComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	~CAnimatedInstanceComponent();

	void Update(float ElapsedTime);
	void Render(CContextManager&  _context);
	void Destroy();
};

