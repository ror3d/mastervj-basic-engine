#pragma once
#include "Component.h"
class CParticleEmitterComponent :
	public CComponent
{
public:
	CParticleEmitterComponent(const std::string& name, CElement* Owner);
	CParticleEmitterComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	~CParticleEmitterComponent();

	void Update(float ElapsedTime);
	void Render(CContextManager&  _context);
	void Destroy();
};

