#pragma once

#include <map>
#include <string>
#include <vector>

#include <Base/Math/Math.h>

#define PHYSX_UPDATE_STEP (1/60.f)

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxCooking;
	class PxMaterial;
	class PxActor;
	class PxControllerManager;
	class PxController;

	namespace debugger
	{
		namespace comm
		{
			class PvdConnection;
		}
	}

	typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;
}

class CPhysXManager
{
protected:
	CPhysXManager();
public:
	enum class ActorType {
		Static,
		Dynamic
	};
	struct ShapeDesc {
		enum class Shape {
			Box,
			Sphere,
			Capsule,
			ConvexMesh,
			TriangleMesh
		} shape;

		Vect3f size;
		float radius;
		float halfHeight;
		Vect3f position;
		Quatf orientation;
		std::string material;
		float density;
		std::vector<uint8>* cookedMeshData;
	};

	static CPhysXManager* CreatePhysXManager();
	virtual ~CPhysXManager();

	void registerMaterial(const std::string& name, float staticFriction, float dynamicFriction, float restitution);

	void createActor(const std::string& name, ActorType type, const ShapeDesc& desc);

	void createPlane(const std::string& name, const std::string& material, Vect4f planeDesc);

	void update(float dt);

	static bool cookConvexMesh(const std::vector<Vect3f>& vec, std::vector<uint8>& outCookedData);

	static bool loadCookedMesh(const std::string& fname, std::vector<uint8>& outCookedData);

	static bool saveCookedMeshToFile(const std::vector<uint8>& inCookedData, const std::string& fname);

protected:

	physx::PxFoundation					*m_Foundation;
	physx::PxPhysics					*m_PhysX;

#if USE_PHYSX_DEBUG
	physx::PxVisualDebuggerConnection	*m_DebugConnection;
#endif

	physx::PxDefaultCpuDispatcher		*m_Dispatcher;
	physx::PxScene						*m_Scene;
	physx::PxCooking					*m_Cooking; // OPTIMIZE OUT
	physx::PxControllerManager			*m_ControllerManager;

private:
	float m_elapsedTime;

	std::map<std::string, physx::PxMaterial*> m_materials;

	struct {
		std::map<std::string, size_t> index;
		std::vector<std::string> name;
		std::vector<Vect3f> position;
		std::vector<Quatf> rotation;
		std::vector<physx::PxActor*> actor;
	} m_actors;
};
