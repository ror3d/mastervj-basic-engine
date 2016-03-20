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
	c->startVelocityRange = make_range(Vect3f(1, 1, 1), Vect3f(-1, 1, -1));
	c->accelerationRange = make_range(Vect3f(0, -2, 0));
	c->sizeRange = make_range(0.1f);
	c->material = CEngine::GetSingleton().getMaterialManager()->get("particle_test");
	c->life = make_range(3.0f);

	c->emitRate = 1;
	add("test", c);
}
