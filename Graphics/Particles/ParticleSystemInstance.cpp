#include "ParticleSystemInstance.h"

#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include "ParticleSystemManager.h"

#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Material/Material.h>

#include "Mesh/RenderableVertexs.h"
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
	Vect3f s = ( rng.second - rng.first );
	Vect3f r(ud(rnde)*s.x, ud(rnde)*s.y, ud(rnde)*s.z);
	return r + rng.first;
}

CColor getRand(std::mt19937 &rnde, std::uniform_real_distribution<float> &ud, range<CColorSpace> &rng)
{
	CColor c1 = rng.first.toHSL();
	CColor c2 = rng.second.toHSL();

	Vect3f hslr = getRand(rnde, ud, make_range(Vect3f(c1.x, c1.y, c1.z), Vect3f(c2.x, c2.y, c2.z)));

	CColorSpace r = CColorSpace(hslr, true);
	r.w = ud(rnde)*(rng.second.w - rng.first.w) + rng.second.w;

	return r.toRGB();
}

CParticleSystemInstance::CParticleSystemInstance(CXMLTreeNode& treeNode)
	: m_activeParticles(0)
	, m_toCreateParticles(0)
	, m_randomEngine(rnd())
	, m_unitDist(0, 1)
	, m_enabled(true)
{
	std::string particleClass = treeNode.GetPszProperty("class", "", true);
	m_particleSystemClass = CEngine::GetSingleton().getParticleManager()->get(particleClass);
	DEBUG_ASSERT(m_particleSystemClass);

	m_particles.resize(MAX_PARTICLES_PER_EMITTER);
	m_vertexs = new CPointsListRenderableVertexs<PARTICLE_VERTEX>(m_particleVtxs, MAX_PARTICLES_PER_EMITTER, MAX_PARTICLES_PER_EMITTER, true);
}

CParticleSystemInstance::CParticleSystemInstance( const CParticleSystemInstance & base )
	: m_activeParticles(0)
	, m_toCreateParticles(0)
	, m_randomEngine(rnd())
	, m_unitDist(0, 1)
	, m_enabled(base.m_enabled)
	, m_particleSystemClass(base.m_particleSystemClass)
	, m_particles(base.m_particles)
	, m_vertexs(base.m_vertexs)
{
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
	if (!m_enabled)
	{
		return;
	}

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

		if (m_particleSystemClass->numFrames > 0)
		{
			p.currentFrame = p.lifetime * m_particleSystemClass->numFrames;
			
			if (m_particleSystemClass->colorInterpolation)
			{
				CColor c1 = m_particleSystemClass->color.first.toHSL();
				CColor c2 = m_particleSystemClass->color.second.toHSL();

				float dist = c2.z - c1.z;
				float increment = dist * p.currentFrame / m_particleSystemClass->numFrames;

				CColorSpace newColor = CColorSpace(c1.x, c1.y, c1.z + increment);

				newColor.HSL = true;

				p.color = newColor.toRGB();
			}
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
		
		if (m_particleSystemClass->colorInterpolation)
		{
			p.color = m_particleSystemClass->color.first;
		}
		else
		{
			p.color = getRand(m_randomEngine, m_unitDist, m_particleSystemClass->color);
		}
	}

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
	if ( m_activeParticles > 1 )
	{
		std::sort( &( m_particleVtxs[0] ), &( m_particleVtxs[m_activeParticles - 1] ),
			[&cameraPos]( PARTICLE_VERTEX &a, PARTICLE_VERTEX &b ) -> bool {
			return ( a.Position - cameraPos ).SquaredLength() > ( b.Position - cameraPos ).SquaredLength();
		} );
	}
}

void CParticleSystemInstance::Render(CContextManager *_context)
{
	if (!m_enabled)
	{
		return;
	}
	auto devCtx = _context->GetDeviceContext();
	m_vertexs->UpdateVertices(devCtx, m_particleVtxs, m_activeParticles);

	auto material = m_particleSystemClass->material;
	auto technique = material->getRenderableObjectTechique()->GetEffectTechnique();

	material->apply();

	m_vertexs->Render(_context, technique);
}
