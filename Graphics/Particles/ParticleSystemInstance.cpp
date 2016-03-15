#include "ParticleSystemInstance.h"

#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>

#include "Renderable/RenderableVertexs.h"
#include "Renderable/RenderableObjectTechnique.h"
#include "Effect/EffectGeometryShader.h"

CParticleSystemInstance::CParticleSystemInstance(CXMLTreeNode& treeNode)
	: CRenderableObject(treeNode)
	, m_activeParticles(0)
	, m_toCreateParticles(0)
{
	std::string particleClass = treeNode.GetPszProperty("particle_class", "", true);
	m_particleSystemClass = CEngine::GetSingleton().getParticleManager()->get(particleClass);
	DEBUG_ASSERT(m_particleSystemClass);

	m_particles.resize(MAX_PARTICLES_PER_EMITTER);
	m_vertexs = new CPointsListRenderableVertexs<PARTICLE_VERTEX>(m_particleVtxs, MAX_PARTICLES_PER_EMITTER, MAX_PARTICLES_PER_EMITTER, true);
}

CParticleSystemInstance::~CParticleSystemInstance()
{
}

void CParticleSystemInstance::Update(float ElapsedTime)
{
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

		if (m_particleSystemClass->timePerFrame > 0)
		{
			p.currentFrame = p.lifetime / m_particleSystemClass->timePerFrame;
		}

	}

	m_toCreateParticles += m_particleSystemClass->emitRate * ElapsedTime;
	size_t n = std::floorf(m_toCreateParticles);
	m_toCreateParticles -= n;

	for (int i = 0; i < n && m_activeParticles < MAX_PARTICLES_PER_EMITTER; ++i)
	{
		ParticleData &p = m_particles[m_activeParticles];
		m_activeParticles++;

		// TODO: Randomize
		p.pos = GetPosition();
		p.vel = m_particleSystemClass->startVelocityRange.first;
		p.acc = m_particleSystemClass->accelerationRange.first;
		p.size = m_particleSystemClass->sizeRange.first;
		p.angle = 0;
		p.currentFrame = 0;
		p.lifetime = 0;
		p.totalLifetime = m_particleSystemClass->life.first;
		// TODO: Rest of the properties
		p.color = CColor(1, 1, 1, 1);
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
}

void CParticleSystemInstance::Render(CContextManager *_context)
{
	auto devCtx = _context->GetDeviceContext();
	static_cast<CPointsListRenderableVertexs<PARTICLE_VERTEX>*>(m_vertexs)->UpdateVertices(devCtx, m_particleVtxs, m_activeParticles);

	auto material = m_particleSystemClass->material;
	auto technique = material->getRenderableObjectTechique()->GetEffectTechnique();

	material->apply();

	m_vertexs->Render(_context, technique);
}
