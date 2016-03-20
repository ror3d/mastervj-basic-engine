#ifndef PARTICLE_SYSTEM_INSTANCE_H
#define PARTICLE_SYSTEM_INSTANCE_H

#include "Renderable/RenderableObject.h"
#include "VertexTypes.h"

#include <Base/Math/Math.h>
#include <Base/Math/Color.h>
#include <vector>
#include <random>

class CParticleSystemClass;
class CRenderableVertexs;


class CParticleSystemInstance : public CRenderableObject
{
public:
	static const int MAX_PARTICLES_PER_EMITTER = 6;
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

public:

	CParticleSystemInstance(CXMLTreeNode& treeNode);

	virtual ~CParticleSystemInstance();
	virtual void Update(float ElapsedTime);
	virtual void Render(CContextManager *_context);

};

#endif
