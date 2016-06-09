#include "AnimatedInstanceComponent.h"


CAnimatedInstanceComponent::CAnimatedInstanceComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CAnimatedInstanceComponent::CAnimatedInstanceComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);
}


CAnimatedInstanceComponent::~CAnimatedInstanceComponent()
{}

void CAnimatedInstanceComponent::Update(float ElapsedTime)
{
}

void CAnimatedInstanceComponent::Render(CContextManager&  _context)
{
}

void CAnimatedInstanceComponent::Destroy()
{
}
