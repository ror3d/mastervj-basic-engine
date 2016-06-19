#ifndef PARTICLE_SYSTEM_MANAGER_H
#define PARTICLE_SYSTEM_MANAGER_H

#include <Base/Utils/TMapManager.h>
#include <Base/Utils/Named.h>
#include <Base/Math/Math.h>
#include <Base/Math/ColorSpace.h>

class CMaterial;
class CXMLTreeNode;

template<typename T>
class range
{
public:
	T first;
	T second;
	std::string params;

	range() : first(), second(), params("") {}
	range(const T &v, const std::string params) : first(v), second(v), params(params) {}
	range(const T &f, const T &s, const std::string params) : first(f), second(s), params(params) {}
	range(const range<T> &r) : first(r.first), second(r.second), params(r.params) {}
};

template<typename T>
range<T> make_range(const T& v, const std::string params = "")
{
	return range<T>(v, v, params);
}
template<typename T>
range<T> make_range(const T& f, const T& s, const std::string params = "")
{
	return range<T>(f, s, params);
}

class CParticleSystemClass : public CNamed
{
public:
	CParticleSystemClass(const std::string& name) : CNamed(name) {}
	CParticleSystemClass(const CXMLTreeNode& node);
	CParticleSystemClass(const CParticleSystemClass& original);
	~CParticleSystemClass();

	CMaterial *material;

	int numFrames;
	bool loopFrames;
	bool colorInterpolation;

	float emitRate;
	range<float> size;
	range<float> life;
	range<Vect3f> startVelocity;
	range<Vect3f> acceleration;
	range<float> startAngle;
	range<float> angleSpeed;
	range<float> angleAcceleration;
	range<CColorSpace> color;
};


class CParticleSystemManager : public TMapManager<CParticleSystemClass>
{
	std::string m_Filename;

public:
	~CParticleSystemManager();

	void Load(const std::string &Filename);

	void reload();

	void writeFile();

	std::map<std::string, CParticleSystemClass*> * getMap() { return &m_resources; }
};


#endif
