#ifndef PARTICLE_SYSTEM_MANAGER_H
#define PARTICLE_SYSTEM_MANAGER_H

#include <Base/Utils/TMapManager.h>
#include <Base/Utils/Named.h>
#include <Base/Math/Math.h>

class CMaterial;
class CXMLTreeNode;

template<typename T>
class range
{
public:
	T first;
	T second;

	range() {}
	range(const T &v) : first(v), second(v) {}
	range(const T &f, const T &s) : first(f), second(s) {}
};

template<typename T>
range<T> make_range(const T& v)
{
	return range<T>(v, v);
}
template<typename T>
range<T> make_range(const T& f, const T& s)
{
	return range<T>(f, s);
}

class CParticleSystemClass : public CNamed
{
public:
	CParticleSystemClass(const std::string& name) : CNamed(name) {}
	CParticleSystemClass(const CXMLTreeNode& node);

	CMaterial *material;

	int numFrames;
	float timePerFrame;
	bool loopFrames;

	float emitRate;
	range<float> sizeRange;
	range<float> life;
	range<Vect3f> startVelocityRange;
	range<Vect3f> accelerationRange;
};


class CParticleSystemManager : public TMapManager<CParticleSystemClass>
{
public:

	void Load(const std::string &Filename);
};


#endif
