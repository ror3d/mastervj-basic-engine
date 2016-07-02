#ifndef PARTICLE_EMITED_COMPONENT_H
#define PARTICLE_EMITED_COMPONENT_H

#include "Component.h"
class CParticleSystemInstance;

class CParticleEmitterComponent :
	public CComponent
{
protected:
	virtual void Init();
	CParticleSystemInstance* m_ParticleInstance;
public:
	CParticleEmitterComponent(const std::string& name, CElement* Owner);
	CParticleEmitterComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	~CParticleEmitterComponent();

	void Update(float ElapsedTime);
	void Render(CContextManager&  _context);
	void Destroy();
	
	CParticleSystemInstance* GetParticleSystemInstance() { return m_ParticleInstance; }
	void SetParticleSystemInstance(CParticleSystemInstance* p) { m_ParticleInstance = p; }
};

#endif
