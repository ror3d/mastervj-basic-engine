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

	unsigned int m_CheckCollisionsEveryNParticles;

public:
	CParticleEmitterComponent(const CParticleEmitterComponent& base, CElement* Owner);
	CParticleEmitterComponent(CXMLTreeNode& node, CElement* Owner);
	~CParticleEmitterComponent();

	void Update(float ElapsedTime);
	void FixedUpdate(float ElapsedTime);
	void Render(CContextManager&  _context);
	void Destroy();

	CParticleSystemInstance* GetParticleSystemInstance() { return m_ParticleInstance; }
	void SetParticleSystemInstance(CParticleSystemInstance* p) { m_ParticleInstance = p; }

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }

	virtual CComponent* Clone(CElement* Owner) const { return new CParticleEmitterComponent( *this, Owner ); }
};

#endif
