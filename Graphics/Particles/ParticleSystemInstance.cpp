#include "ParticleSystemInstance.h"

#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>

#include "Renderable/RenderableVertexs.h"
#include "Renderable/RenderableObjectTechnique.h"
#include "Effect/EffectGeometryShader.h"

#include <algorithm>

std::random_device rnd;

float getRand(std::mt19937 &rnde, std::uniform_real_distribution<float> &ud, range<float> &rng)
{
	float r = ud(rnde);
	return ((rng.second - rng.first) * r) + rng.first;
}

Vect3f getRand(std::mt19937 &rnde, std::uniform_real_distribution<float> &ud, range<Vect3f> &rng)
{
	Vect3f r(ud(rnde), ud(rnde), ud(rnde));
	return ((rng.second - rng.first) * r) + rng.first;
}

CColor getRand(std::mt19937 &rnde, std::uniform_real_distribution<float> &ud, range<CColor> &rng)
{
	CColor r(ud(rnde), ud(rnde), ud(rnde), ud(rnde));
	return ((rng.second - rng.first) * r) + rng.first;
}

CParticleSystemInstance::CParticleSystemInstance(CXMLTreeNode& treeNode)
	: CRenderableObject(treeNode)
	, m_activeParticles(0)
	, m_toCreateParticles(0)
	, m_randomEngine(rnd())
	, m_unitDist(0, 1)
{
	std::string particleClass = treeNode.GetPszProperty("particle_class", "", true);
	m_particleSystemClass = CEngine::GetSingleton().getParticleManager()->get(particleClass);
	DEBUG_ASSERT(m_particleSystemClass);

	m_particles.resize(MAX_PARTICLES_PER_EMITTER);
	m_vertexs = new CPointsListRenderableVertexs<PARTICLE_VERTEX>(m_particleVtxs, MAX_PARTICLES_PER_EMITTER, MAX_PARTICLES_PER_EMITTER, true);
}

CParticleSystemInstance::~CParticleSystemInstance()
{
	if (m_vertexs)
	{
		delete m_vertexs;
		m_vertexs = nullptr;
	}
}

void CParticleSystemInstance::Update(float ElapsedTime)
{
	// Update existing particles
	for (int i = 0; i < m_activeParticles; ++i)
	{
		ParticleData &p = m_particles[i];
		p.lifetime += ElapsedTime;
		if (p.lifetime >= p.totalLifetime)
		{
			m_activeParticles--;
			p = m_particles[m_activeParticles];
			--i;
			continue;
		}
		p.vel += p.acc * ElapsedTime;
		p.pos += p.vel * ElapsedTime;
		p.angularSpeed += p.angularAcc * ElapsedTime;
		p.angle += p.angularSpeed * ElapsedTime;

		if (m_particleSystemClass->timePerFrame > 0 && m_particleSystemClass->timePerFrame > 0)
		{
			p.currentFrame = p.lifetime / m_particleSystemClass->timePerFrame;
		}

	}

	m_toCreateParticles += m_particleSystemClass->emitRate * ElapsedTime;
	size_t n = std::floorf(m_toCreateParticles);
	m_toCreateParticles -= n;

	// Create new particles
	for (int i = 0; i < n && m_activeParticles < MAX_PARTICLES_PER_EMITTER; ++i)
	{
		ParticleData &p = m_particles[m_activeParticles];
		m_activeParticles++;

		p.pos = GetPosition();
		p.vel = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->startVelocity);
		p.acc = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->acceleration);
		p.size = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->size);
		p.angle = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->startAngle);
		p.angularSpeed = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->angleSpeed);
		p.angularAcc = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->angleAcceleration);
		p.currentFrame = 0;
		p.lifetime = 0;
		p.totalLifetime = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->life);
		p.color = getRand( m_randomEngine, m_unitDist, m_particleSystemClass->color );
	}

	// TODO: sort particles

	// Copy particles
	for (int i = 0; i < m_activeParticles; ++i)
	{
		ParticleData &p = m_particles[i];
		m_particleVtxs[i].Color = p.color;
		m_particleVtxs[i].Position = p.pos;
		m_particleVtxs[i].UV.x = p.size;
		m_particleVtxs[i].UV.y = p.angle;
		m_particleVtxs[i].UV2.x = p.currentFrame;
	}
	Vect4f cameraPos4 = CEffectManager::m_SceneParameters.m_CameraPosition;
	Vect3f cameraPos( cameraPos4.x, cameraPos4.y, cameraPos4.z );
	std::sort( &( m_particleVtxs[0] ), &( m_particleVtxs[MAX_PARTICLES_PER_EMITTER - 1] ),
		[&cameraPos]( PARTICLE_VERTEX &a, PARTICLE_VERTEX &b ) -> bool {
			return ( a.Position - cameraPos ).SquaredLength() < ( b.Position - cameraPos ).SquaredLength();
	} );
}

void CParticleSystemInstance::Render(CContextManager *_context)
{
	auto devCtx = _context->GetDeviceContext();
	m_vertexs->UpdateVertices(devCtx, m_particleVtxs, m_activeParticles);

	auto material = m_particleSystemClass->material;
	auto technique = material->getRenderableObjectTechique()->GetEffectTechnique();

	material->apply();

	m_vertexs->Render(_context, technique);
}
