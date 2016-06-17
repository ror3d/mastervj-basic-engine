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
	bool m_enabled;

public:

	CParticleSystemInstance(CXMLTreeNode& treeNode);

	virtual ~CParticleSystemInstance();
	virtual void Update(float ElapsedTime);
	virtual void Render(CContextManager *_context);

	inline void SetPosition(Vect3f pos) { m_position = pos; }
	inline Vect3f GetPosition() const { return m_position; }
	inline void SetEnabled(bool e) { m_enabled = e; }
	inline bool IsEnabled() const { return m_enabled; }
};

#endif
