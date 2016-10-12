#pragma once

#include <map>
#include <string>
#include <vector>
#include <set>
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
	float mass;
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

	bool m_enabledTriggerDetection;

	static CPhysXManager* CreatePhysXManager();
	virtual ~CPhysXManager();

	void InitPhysx();

	void update(float dt);

	void registerMaterial(const std::string& name, float staticFriction, float dynamicFriction, float restitution);

	void createActor(const std::string& name, ActorType type, const CPhysxColliderShapeDesc& desc, bool isKinematic, bool isTrigger);

	void destroyActor(const std::string& name);

	void MoveActor(std::string name, Vect3f position, Quatf rotation);

	void setPosition(Vect3f pos, const std::string &name);

	void createPlane(const std::string& name, const std::string& material, Vect4f planeDesc);

	void createController(float height, float radius, float density, Vect3f pos, std::string name);

	Vect3f moveCharacterController(Vect3f displacement, Vect3f up, float elapsedTime, const std::string &name, bool stickToGround);

	bool isCharacterControllerGrounded(const std::string &name);

	void resizeCharacterController( const std::string &name, float height, float radius );

	void releaseCharacterController(const std::string &name);

	void releaseCharacterControllers();

	Vect3f getActorPosition( const std::string& name );
	Quatf getActorRotation( const std::string& name );

	std::map<std::string, physx::PxController*> getCharControllers(){ return m_CharacterControllers;  }

	bool RayCast(Vect3f origin, Vect3f direction, float distance, Vect3f& out_hitPosition);
	std::string CPhysXManager::RayCastName(Vect3f origin, Vect3f direction, float distance, std::string objectToAvoid);

	std::set<std::string> getTriggerCollisions(const std::string& triggerName) { return m_TriggerCollisions[triggerName]; }

	//std::set<std::string> getTriggerCollisions( const std::string& triggerName ) { auto ret = std::move( m_TriggerCollisions[triggerName] ); return ret; }

	std::set<std::string> getActorCollisions( const std::string& actorName ) { auto ret = std::move( m_ActorCollisions[actorName] ); return ret; }


	bool cookConvexMesh(const std::vector<Vect3f>& vec, std::vector<uint8> * outCookedData);

	bool cookTriangleMesh(const std::vector<Vect3f>& vec, const std::vector<unsigned short>& ind, std::vector<uint8> * outCookedData, unsigned short indexSize);

	bool loadCookedMesh(const std::string& fname, std::vector<uint8>& outCookedData);

	bool saveCookedMeshToFile(const std::vector<uint8>& inCookedData, const std::string& fname);


	void destroy() {}

protected:

	struct {
		std::map<std::string, size_t> index;
		std::vector<std::string> name;
		std::vector<Vect3f> position;
		std::vector<Quatf> rotation;
		std::vector<physx::PxActor*> actor;
	} m_actors;

	std::map<std::string, std::set<std::string>> m_TriggerCollisions;
	std::map<std::string, std::set<std::string>> m_ActorCollisions;


	physx::PxFoundation					*m_Foundation;
	physx::PxPhysics					*m_PhysX;

#if USE_PHYSX_DEBUG
	physx::PxVisualDebuggerConnection	*m_DebugConnection;
#endif

	physx::PxDefaultCpuDispatcher		*m_Dispatcher;
	physx::PxScene						*m_Scene;
	physx::PxCooking					*m_Cooking; // OPTIMIZE OUT
	physx::PxControllerManager			*m_ControllerManager;

	std::map<size_t, std::string> m_CharacterControllerIdxs;
private:
	float m_elapsedTime;

	std::map<std::string, physx::PxMaterial*> m_materials;
	std::map<std::string, physx::PxController*> m_CharacterControllers;

	// Map < cc_name, surface_actor_id >
	std::map<std::string, size_t> m_CharacterControllerSurface;
	// Map < cc_name, surface_displacement >
	std::map<std::string, Vect3f> m_CharacterControllerDisplacements;

	size_t m_CharacterControllerLastIdx;


};
