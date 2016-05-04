#pragma once

#include <map>
#include <string>
#include <vector>
#include <Base/Math/Math.h>

#define USE_PHYSX_DEBUG 1

#define PHYSX_UPDATE_STEP (1/60.f)

class CRenderableVertexs;

namespace physx
{
	class PxFoundation;
	class PxShape;
	class PxGeometry;
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

struct CPhysxColliderShapeDesc {
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
	std::vector<uint8> *cookedMeshData;
};

class CPhysXManager
{
protected:
	CPhysXManager();
public:
	enum class ActorType {
		Static,
		Dynamic
	};

	static CPhysXManager* CreatePhysXManager();
	virtual ~CPhysXManager();

	void registerMaterial(const std::string& name, float staticFriction, float dynamicFriction, float restitution);

	void createActor(const std::string& name, ActorType type, const CPhysxColliderShapeDesc& desc);

	void createPlane(const std::string& name, const std::string& material, Vect4f planeDesc);

	physx::PxShape* CPhysXManager::createStatic(const std::string& name, const std::string& material, Vect3f position, Quatf orientation, Vect3f size, bool trigger, CPhysxColliderShapeDesc::Shape shape);

	void CPhysXManager::createStaticBox(const std::string name, Vect3f size, const std::string Material, Vect3f position, Quatf orientation, bool trigger);

	void CPhysXManager::createStaticSphere(const std::string name, Vect3f size, const std::string Material, Vect3f position, Quatf orientation, bool trigger);

	void InitPhysx();

	void createController(float height, float radius, float density, Vect3f pos, std::string name);

	void update(float dt);

	bool cookConvexMesh(const std::vector<Vect3f>& vec, std::vector<uint8> * outCookedData);

	bool cookTriangleMesh(const std::vector<Vect3f>& vec, const std::vector<unsigned short>& ind, std::vector<uint8> * outCookedData, unsigned short indexSize);

	bool loadCookedMesh(const std::string& fname, std::vector<uint8>& outCookedData);

	bool saveCookedMeshToFile(const std::vector<uint8>& inCookedData, const std::string& fname);

	Vect3f moveCharacterController(Vect3f movement, Vect3f direction, float _ElapsedTime, std::string name);

	void releaseCharacterControllers();

	std::map<std::string, physx::PxController*> getCharControllers(){ return m_CharacterControllers;  }

	void destroy() {}

	struct {
		std::map<std::string, size_t> index;
		std::vector<std::string> name;
		std::vector<Vect3f> position;
		std::vector<Quatf> rotation;
		std::vector<physx::PxActor*> actor;
	} m_actors;

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
	std::map<std::string, physx::PxController*> m_CharacterControllers;


};
