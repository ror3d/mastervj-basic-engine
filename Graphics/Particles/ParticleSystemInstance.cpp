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

float Hue_2_RGB(float v1, float v2, float vH)             //Function Hue_2_RGB
{
	if (vH < 0) vH += 1;
	if (vH > 1) vH -= 1;
	if ((6 * vH) < 1) return (v1 + (v2 - v1) * 6 * vH);
	if ((2 * vH) < 1) return (v2);
	if ((3 * vH) < 2) return (v1 + (v2 - v1) * ((2 / 3) - vH) * 6);
	return (v1);
}

CColor hsl2rgb(Vect3f Color_HSL)
{
	float H = Color_HSL.x * 360;
	float S = Color_HSL.y;
	float L = Color_HSL.z;
	
	Vect3f rgb;

	float c = (1 - abs(2*L-1)) * S;
	float x = c * (1 - abs(fmodf(H / 60, 2) - 1));
	float m = L - c / 2;

	if (H < 60)
	{
		rgb.x = c;
		rgb.y = x;
		rgb.z = 0;
	}
	else if (H < 120)
	{
		rgb.x = x;
		rgb.y = c;
		rgb.z = 0;
	}
	else if (H < 180)
	{
		rgb.x = 0;
		rgb.y = c;
		rgb.z = x;
	}
	else if (H < 240)
	{
		rgb.x = 0;
		rgb.y = x;
		rgb.z = c;
	}
	else if (H < 300)
	{
		rgb.x = x;
		rgb.y = 0;
		rgb.z = c;
	}
	else if (H < 360)
	{
		rgb.x = c;
		rgb.y = 0;
		rgb.z = x;
	}

	CColor r;
	r.x = rgb.x + m;
	r.y = rgb.y + m;
	r.z = rgb.z + m;
	r.w = 1.0f;

	return r;

}

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

CColor getRand(std::mt19937 &rnde, std::uniform_real_distribution<float> &ud, range<CColor> &rng)
{
	CColor hsl1 = rng.first.rgb2hsl();
	CColor hsl2 = rng.second.rgb2hsl();
	Vect3f hslr = getRand(rnde, ud, make_range(Vect3f(hsl1.x, hsl1.y, hsl1.z), Vect3f(hsl2.x, hsl2.y, hsl2.z)));
	CColor r = hsl2rgb(hslr);
	r.w = ud(rnde)*(rng.second.w - rng.first.w) + rng.second.w;
	return r;
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
	auto devCtx = _context->GetDeviceContext();
	m_vertexs->UpdateVertices(devCtx, m_particleVtxs, m_activeParticles);

	auto material = m_particleSystemClass->material;
	auto technique = material->getRenderableObjectTechique()->GetEffectTechnique();

	material->apply();

	m_vertexs->Render(_context, technique);
}
