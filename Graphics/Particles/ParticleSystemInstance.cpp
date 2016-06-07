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

template<typename T>
Vect3f rgb2hsl(T Color_RGB)
{
	float var_Min = min(Color_RGB.x, Color_RGB.y, Color_RGB.z);    //Min. value of RGB
	float var_Max = max(Color_RGB.x, Color_RGB.y, Color_RGB.z);    //Max. value of RGB
	float del_Max = var_Max - var_Min;             //Delta RGB value

	float L = (var_Max + var_Min) / 2;

	float H;
	float S;

	if (del_Max == 0)                     //This is a gray, no chroma...
		{
			H = 0;                                //HSL results from 0 to 1
			S = 0;
		}
	else                                    //Chromatic data...
	{
		if (L < 0.5){
			S = del_Max / (var_Max + var_Min);
		}
		else
		{
			S = del_Max / (2 - var_Max - var_Min);
		}

		float del_R = (((var_Max - Color_RGB.x) / 6) + (del_Max / 2)) / del_Max;
		float del_G = (((var_Max - Color_RGB.y) / 6) + (del_Max / 2)) / del_Max;
		float del_B = (((var_Max - Color_RGB.z) / 6) + (del_Max / 2)) / del_Max;

		if (Color_RGB.x == var_Max)
		{
			H = del_B - del_G;
		}

		else if (Color_RGB.y == var_Max)
		{
			H = (1 / 3) + del_R - del_B;
		}

		else if (Color_RGB.z == var_Max)
		{
			H = (2 / 3) + del_G - del_R;
		}

		if (H < 0) H += 1;
		if (H > 1) H -= 1;
	}
	return Vect3f(H, S, L);
}


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
	float R;
	float G;
	float B;
	float var_2;
	float var_1;

	if (Color_HSL.y == 0)                       //HSL from 0 to 1
	{
		R = Color_HSL.z;                      //RGB results from 0 to 255
		G = Color_HSL.z;
		B = Color_HSL.z;
	}
	else
	{
		if (Color_HSL.z < 0.5)
		{
			var_2 = Color_HSL.z * (1 + Color_HSL.y);
		}
		else
		{
			var_2 = (Color_HSL.z + Color_HSL.y) - (Color_HSL.y * Color_HSL.z);
		}

		var_1 = 2 * Color_HSL.z - var_2;

		R = Hue_2_RGB(var_1, var_2, Color_HSL.x + (1 / 3));
		G = Hue_2_RGB(var_1, var_2, Color_HSL.y);
		B = Hue_2_RGB(var_1, var_2, Color_HSL.z - (1 / 3));
	}
	CColor r;
	r.SetRed(R);
	r.SetGreen(G);
	r.SetBlue(B);
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
	Vect3f hsl1 = rgb2hsl(rng.first);
	Vect3f hsl2 = rgb2hsl(rng.second);
	Vect3f hslr = getRand(rnde, ud, make_range(hsl1, hsl2));
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
