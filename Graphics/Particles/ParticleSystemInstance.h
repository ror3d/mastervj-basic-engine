#ifndef PARTICLE_SYSTEM_INSTANCE_H
#define PARTICLE_SYSTEM_INSTANCE_H

#include "Mesh/VertexTypes.h"

#include "Renderable/Renderable.h"
#include <Base/Math/Math.h>
#include <Base/Math/Color.h>
#include <vector>
#include <random>

class CParticleSystemClass;
class CRenderableVertexs;
class CXMLTreeNode;
class CContextManager;


class CParticleSystemInstance : public IRenderable
{
public:
	static const int MAX_PARTICLES_PER_EMITTER = 200;
private:
	struct ParticleData
	{
		Vect3f pos, vel, acc;
		int currentFrame;
		float timeToNextFrame;
		float lifetime, totalLifetime;
		float angle, angularSpeed, angularAcc;
		CColor color;
		float size;
	};

	float m_toCreateParticles;

	size_t m_activeParticles;
	std::vector<ParticleData> m_particles;

	CParticleSystemClass * m_particleSystemClass;


	PARTICLE_VERTEX m_particleVtxs[MAX_PARTICLES_PER_EMITTER];

	CRenderableVertexs *m_vertexs;

	std::mt19937 m_randomEngine;
	std::uniform_real_distribution<float> m_unitDist;

	Vect3f m_position;
	Quatf m_startSpeedRotation;
	bool m_enabled;

public:
	CParticleSystemInstance(CXMLTreeNode& treeNode);
	CParticleSystemInstance(const CParticleSystemInstance& base);

	virtual ~CParticleSystemInstance();
	virtual void Update(float ElapsedTime);
	virtual void Render(CContextManager *_context);
	virtual CParticleSystemClass* getParticleClass() { return m_particleSystemClass; }
	virtual void setParticleClass(CParticleSystemClass* particleClass)
	{
		m_particleSystemClass = particleClass;
	}

	inline void SetPosition(Vect3f pos) { m_position = pos; }
	inline Vect3f GetPosition() const { return m_position; }
	inline void SetStartSpeedRotation(Quatf ssr) { m_startSpeedRotation = ssr; }
	inline Quatf GetStartSpeedRotation() const { return m_startSpeedRotation; }
	inline void SetEnabled(bool e) { m_enabled = e; }
	inline bool IsEnabled() const { return m_enabled; }

	std::vector<std::string> CheckCollisions( unsigned int step );
};

#endif
