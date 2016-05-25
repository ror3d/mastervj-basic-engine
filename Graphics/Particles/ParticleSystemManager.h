#ifndef PARTICLE_SYSTEM_MANAGER_H
#define PARTICLE_SYSTEM_MANAGER_H

#include <Base/Utils/TMapManager.h>
#include <Base/Utils/Named.h>
#include <Base/Math/Math.h>
#include <Base/Math/Color.h>

class CMaterial;
class CXMLTreeNode;

template<typename T>
class range
{
public:
	T first;
	T second;

	range() : first(), second() {}
	range(const T &v) : first(v), second(v) {}
	range(const T &f, const T &s) : first(f), second(s) {}
	range(const range<T> &r) : first(r.first), second(r.second) {}
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
	CParticleSystemClass(const CParticleSystemClass& original);

	CMaterial *material;

	int numFrames;
	float timePerFrame;
	bool loopFrames;

	float emitRate;
	range<float> size;
	range<float> life;
	range<Vect3f> startVelocity;
	range<Vect3f> acceleration;
	range<float> startAngle;
	range<float> angleSpeed;
	range<float> angleAcceleration;
	range<CColor> color;
};


class CParticleSystemManager : public TMapManager<CParticleSystemClass>
{
	std::string m_Filename;

public:

	void Load(const std::string &Filename);

	void reload();

	void writeFile();

	std::map<std::string, CParticleSystemClass*> * getMap() { return &m_resources; }
};


#endif
