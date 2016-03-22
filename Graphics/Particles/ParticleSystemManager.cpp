#include "ParticleSystemManager.h"

#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>

CParticleSystemClass::CParticleSystemClass(const CXMLTreeNode& node)
	: CNamed(node)
{
	// TODO
}

void CParticleSystemManager::Load(const std::string &Filename)
{
	CParticleSystemClass * c = new CParticleSystemClass("test");

	c->numFrames = 1;
	c->startVelocity = make_range(Vect3f(1, 1, 1), Vect3f(-1, 1, -1));
	c->acceleration = make_range(Vect3f(0, -2, 0));
	c->size = make_range(0.1f, 0.3f);
	c->startAngle = make_range( 0.f, 3.1415f );
	c->material = CEngine::GetSingleton().getMaterialManager()->get("particle_test");
	c->life = make_range(3.0f, 6.0f);

	c->emitRate = 10;
	add("test", c);
}
