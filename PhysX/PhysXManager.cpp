#include "PhysXManager.h"
#include <Base/Math/Math.h>
#include <Base/Utils/Utils.h>
#include <PxPhysicsAPI.h>

#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <Core/Trigger/TriggerManager.h>

#include <fstream>
#include <iterator>
#include <algorithm>

#if (defined(_MSC_VER) && _MSC_VER >= 1800) || __cplusplus <= 199711L
#include <thread>
#endif
#if defined(PX_WINDOWS)
#include <Windows.h>
#endif

#include <Core/Engine/Engine.h>

#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysXProfileSDKDEBUG.lib")
#pragma comment(lib, "PhysX3CharacterKinematicDEBUG_x86.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x86")

/* // Not used
#pragma comment(lib, "LowLevelClothDEBUG.lib")
#pragma comment(lib, "LowLevelDEBUG.lib")
#pragma comment(lib, "PhysX3GpuDEBUG_x86.lib")
#pragma comment(lib, "PhysX3VehicleDEBUG.lib")
#pragma comment(lib, "PvdRuntimeDEBUG.lib")
#pragma comment(lib, "PxTaskDEBUG.lib")
#pragma comment(lib, "SceneQueryDEBUG.lib")
#pragma comment(lib, "SimulationControllerDEBUG.lib")
*/
#else
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysXProfileSDK.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x86.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDK.lib")
#pragma comment(lib, "PhysX3Cooking_x86")
#endif
#define USE_PHYSX_DEBUG 1
#if USE_PHYSX_DEBUG
#define				PVD_HOST			"127.0.0.1"
#endif

#ifdef CHECKED_RELEASE
#undef CHECKED_RELEASE
#endif
#define CHECKED_RELEASE(x) if(x!=nullptr) { x->release(); x=nullptr; }

inline physx::PxVec3 v(const Vect3f& v)
{ return physx::PxVec3(v.x, v.y, v.z); }
inline Vect3f v(const physx::PxVec3& v)
{ return Vect3f(v.x, v.y, v.z); }
inline Vect3f v(const physx::PxExtendedVec3& v)
{ return Vect3f((float)v.x, (float)v.y, (float)v.z); }

inline Quatf q(const physx::PxQuat& q)
{ return Quatf(q.x, q.y, q.z, q.w); }
inline physx::PxQuat q(const Quatf& q)
{ return physx::PxQuat(q.x, q.y, q.z, q.w); }


class CPhysXManagerImplementation
	: public CPhysXManager
	, public physx::PxSimulationEventCallback
	, public physx::PxUserControllerHitReport
{
public:
	CPhysXManagerImplementation();
	~CPhysXManagerImplementation();

	// PxSimulationEventCallback
	void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count);
	void onWake(physx::PxActor **actors, physx::PxU32 count);
	void onSleep(physx::PxActor **actors, physx::PxU32 count);
	void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs);
	void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

	// PxUserControllerHitReport
	void onShapeHit(const physx::PxControllerShapeHit &hit);
	void onControllerHit(const physx::PxControllersHit &hit);
	void onObstacleHit(const physx::PxControllerObstacleHit &hit);

};

CPhysXManager* CPhysXManager::CreatePhysXManager()
{
	return new CPhysXManagerImplementation();
}


static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

CPhysXManagerImplementation::CPhysXManagerImplementation()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	DEBUG_ASSERT(m_Foundation);

	physx::PxProfileZoneManager* profileZoneManager = nullptr;
#if USE_PHYSX_DEBUG
	profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_Foundation);
#endif
	m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, profileZoneManager);
	DEBUG_ASSERT(m_PhysX);

	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	DEBUG_ASSERT(m_Cooking);

#if USE_PHYSX_DEBUG
	if (m_PhysX->getPvdConnectionManager())
	{
		m_PhysX->getVisualDebugger()->setVisualizeConstraints(true);
		m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		m_DebugConnection = physx::PxVisualDebuggerExt::createConnection(m_PhysX->getPvdConnectionManager(), PVD_HOST, 5425, 100);
	}
	else
	{
		m_DebugConnection = nullptr;
	}
#endif

	int ncpus = 0;

#if (defined(_MSC_VER) && _MSC_VER >= 1800) || __cplusplus <= 199711L
	ncpus = std::thread::hardware_concurrency();
#endif
	if (ncpus == 0)
	{
#if defined(PX_WINDOWS)
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);

		ncpus = sysinfo.dwNumberOfProcessors;
#endif
	}

	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(ncpus);


	physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0, -9.81, 0);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	m_Scene = m_PhysX->createScene(sceneDesc);
	DEBUG_ASSERT(m_Scene);

	m_Scene->setSimulationEventCallback(this);


	m_ControllerManager = PxCreateControllerManager(*m_Scene);
	m_ControllerManager->setOverlapRecoveryModule(true);


}

CPhysXManagerImplementation::~CPhysXManagerImplementation()
{


}

// PxSimulationEventCallback
void CPhysXManagerImplementation::onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count)
{
}

void CPhysXManagerImplementation::onWake(physx::PxActor **actors, physx::PxU32 count)
{
}

void CPhysXManagerImplementation::onSleep(physx::PxActor **actors, physx::PxU32 count)
{
}

void CPhysXManagerImplementation::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs)
{
}

void CPhysXManagerImplementation::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		if ((pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)))
			continue;

		size_t l_indexTrigger = (size_t)pairs[i].triggerActor->userData;
		//size_t l_indexActor = (size_t)pairs[i].otherActor->userData;

		std::string l_triggerName = m_actors.name[l_indexTrigger];

		CEngine::GetSingleton().getTriggerManager()->Activate(l_triggerName);
	}
}


// PxUserControllerHitReport
void CPhysXManagerImplementation::onShapeHit(const physx::PxControllerShapeHit &hit)
{
}

void CPhysXManagerImplementation::onControllerHit(const physx::PxControllersHit &hit)
{
}

void CPhysXManagerImplementation::onObstacleHit(const physx::PxControllerObstacleHit &hit)
{
}

CPhysXManager::CPhysXManager()
	: m_elapsedTime(0)
	, m_Foundation(nullptr)
	, m_PhysX(nullptr)

#if USE_PHYSX_DEBUG
	, m_DebugConnection(nullptr)
#endif
	, m_Dispatcher(nullptr)
	, m_Scene(nullptr)
	, m_Cooking(nullptr)
	, m_ControllerManager(nullptr)
{
}

CPhysXManager::~CPhysXManager()
{
	for (auto &pair : m_materials)
	{
		CHECKED_RELEASE(pair.second);
	}
	m_materials.clear();
	releaseCharacterControllers();
	CHECKED_RELEASE(m_ControllerManager);
	CHECKED_RELEASE(m_Scene);
	CHECKED_RELEASE(m_Dispatcher);
#if USE_PHYSX_DEBUG
	CHECKED_RELEASE(m_DebugConnection);
#endif
	CHECKED_RELEASE(m_Cooking);
	auto profileZoneManager = m_PhysX->getProfileZoneManager();
	CHECKED_RELEASE(m_PhysX);
	CHECKED_RELEASE(profileZoneManager);
	CHECKED_RELEASE(m_Foundation);
}

void CPhysXManager::registerMaterial(const std::string& name, float staticFriction, float dynamicFriction, float restitution)
{
	auto it = m_materials.find(name);
	if (it != m_materials.end())
	{
		it->second->release();
	}
	m_materials[name] = m_PhysX->createMaterial(staticFriction, dynamicFriction, restitution);
}

bool CPhysXManager::cookConvexMesh(const std::vector<Vect3f>& vec, std::vector<uint8> * outCookedData)
{
	physx::PxConvexMeshDesc meshDesc;

	meshDesc.points.count = vec.size();
	meshDesc.points.stride = sizeof(Vect3f);
	meshDesc.points.data = vec.data();

	meshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream oBuf;
	physx::PxConvexMeshCookingResult::Enum result;
	bool success = m_Cooking->cookConvexMesh(meshDesc, oBuf, &result);
	DEBUG_ASSERT(success);
	outCookedData->assign(oBuf.getData(), oBuf.getData() + oBuf.getSize());
	return success;
}

bool CPhysXManager::cookTriangleMesh(const std::vector<Vect3f>& vec, const std::vector<unsigned short>& ind, std::vector<uint8> * outCookedData, unsigned short indexSize){
	physx::PxTriangleMeshDesc meshDesc;

	meshDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

	meshDesc.points.count = vec.size();
	meshDesc.points.stride = sizeof(Vect3f);
	meshDesc.points.data = vec.data();

	meshDesc.triangles.count = ind.size()/3;
	meshDesc.triangles.stride = sizeof(physx::PxU16) * 3;
	meshDesc.triangles.data = ind.data();

	physx::PxDefaultMemoryOutputStream oBuf;
	bool success = m_Cooking->cookTriangleMesh(meshDesc, oBuf);
	DEBUG_ASSERT(success);

	outCookedData->assign(oBuf.getData(), oBuf.getData() + oBuf.getSize());
	return success;
}


bool CPhysXManager::loadCookedMesh(const std::string& fname, std::vector<uint8>& outCookedData)
{
	std::basic_ifstream<uint8> f(fname, std::ios::in | std::ifstream::binary);

	if (f)
	{
		f.seekg( 0, f.end );
		size_t length = f.tellg();
		f.seekg( 0, f.beg );

		uint8 *data = new uint8[length];
		f.read( data, length );
		std::copy( data, data+length, std::back_inserter( outCookedData ) );
		delete[] data;

		return true;
	}

	return false;
}

bool CPhysXManager::saveCookedMeshToFile(const std::vector<uint8>& inCookedData, const std::string& fname)
{
	std::basic_ofstream<uint8> f(fname, std::ios::out | std::ifstream::binary);

	if (f)
	{
		std::ostream_iterator<uint8, uint8> iter(f);
		std::copy(inCookedData.begin(), inCookedData.end(), iter);
		return true;
	}

	return false;
}


void CPhysXManager::createPlane(const std::string& name, const std::string& material, Vect4f planeDesc)
{
	auto idx = m_actors.actor.size();

	auto matIt = m_materials.find(material);
	DEBUG_ASSERT(matIt != m_materials.end());

	physx::PxMaterial* mat = matIt->second;

	auto groundPlane = physx::PxCreatePlane(*m_PhysX, physx::PxPlane(planeDesc.x, planeDesc.y, planeDesc.z, planeDesc.w), *mat);

	physx::PxShape* shape;

	size_t nShapes = groundPlane->getShapes(&shape, 1);
	DEBUG_ASSERT(nShapes == 1);

	groundPlane->userData = reinterpret_cast<void*>(idx);

	m_Scene->addActor(*groundPlane);

	DEBUG_ASSERT(m_actors.actor.size() == m_actors.index.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.name.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.position.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.rotation.size());

	m_actors.index[name] = idx;
	m_actors.name.push_back(name);
	Vect3f normal = Vect3f(planeDesc.x, planeDesc.y, planeDesc.z);
	m_actors.position.push_back(normal*planeDesc.w);
	m_actors.rotation.push_back(Quatf::ShortestArc(Vect3f(0, 1, 0), normal));
	m_actors.actor.push_back(groundPlane);

}

physx::PxShape* CPhysXManager::createStatic(const std::string& name, const std::string& material, Vect3f position, Quatf orientation, Vect3f size, bool trigger, CPhysxColliderShapeDesc::Shape figure)
{
	auto idx = m_actors.actor.size();

	auto matIt = m_materials.find(material);
	DEBUG_ASSERT(matIt != m_materials.end());

	physx::PxMaterial* mat = matIt->second;
	physx::PxShape* shape;

	switch (figure)
	{
	case CPhysxColliderShapeDesc::Shape::Box:
		shape = m_PhysX->createShape(physx::PxBoxGeometry(mathUtils::Abs(size.x / 2), mathUtils::Abs(size.y / 2), mathUtils::Abs(size.z / 2)), *mat);
		break;

	case CPhysxColliderShapeDesc::Shape::Sphere:
		shape = m_PhysX->createShape(physx::PxSphereGeometry(size.x/2), *mat);
		break;

	default:
		throw std::logic_error("Not yet implemented");
		break;
	}

	if (trigger)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	physx::PxVec3 Vec = physx::PxVec3(position.x, position.y, position.z);
	physx::PxQuat Quat = physx::PxQuat(orientation.x, orientation.y, orientation.z, orientation.w);
	physx::PxTransform Transform = physx::PxTransform(Vec, Quat);
	physx::PxRigidStatic* StaticBody = m_PhysX->createRigidStatic(Transform);

	StaticBody->attachShape(*shape);

	StaticBody->userData = reinterpret_cast<void*>(idx);
	m_Scene->addActor(*StaticBody);
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.index.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.name.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.position.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.rotation.size());

	m_actors.index[name] = idx;
	m_actors.name.push_back(name);

	m_actors.position.push_back(position);
	m_actors.rotation.push_back(orientation);
	m_actors.actor.push_back(StaticBody);

	return shape;
}


void CPhysXManager::createStaticBox(const std::string name, Vect3f size, const std::string Material, Vect3f position, Quatf orientation, bool trigger)
{
	physx::PxShape* shape = createStatic(name, Material, position, orientation, size, trigger, CPhysxColliderShapeDesc::Shape::Box);

	shape->release();
}

void CPhysXManager::createStaticSphere(const std::string name, Vect3f size, const std::string Material, Vect3f position, Quatf orientation, bool trigger)
{
	physx::PxShape* shape = createStatic(name, Material, position, orientation, size, trigger, CPhysxColliderShapeDesc::Shape::Sphere);

	shape->release();
}

void CPhysXManager::createActor(const std::string& name, ActorType actorType, const CPhysxColliderShapeDesc& desc, bool isKinematic)
{
	auto idx = m_actors.actor.size();

	auto matIt = m_materials.find(desc.material);
	DEBUG_ASSERT(matIt != m_materials.end());

	physx::PxMaterial* mat = matIt->second;

	physx::PxGeometry* geom = nullptr;
	switch (desc.shape)
	{
		case CPhysxColliderShapeDesc::Shape::Box:
			geom = new physx::PxBoxGeometry(desc.size.x, desc.size.y, desc.size.z);
			break;

		case CPhysxColliderShapeDesc::Shape::Sphere:
			geom = new physx::PxSphereGeometry(desc.radius);
			break;

		case CPhysxColliderShapeDesc::Shape::Capsule:
			geom = new physx::PxCapsuleGeometry(desc.radius, desc.halfHeight);
			break;

		case CPhysxColliderShapeDesc::Shape::ConvexMesh:
		{
			physx::PxDefaultMemoryInputData input(desc.cookedMeshData->data(), desc.cookedMeshData->size());
			physx::PxConvexMesh *mesh = m_PhysX->createConvexMesh(input);
			physx::PxMeshScale scale(physx::PxVec3(mathUtils::Abs(desc.size.x), mathUtils::Abs(desc.size.y), mathUtils::Abs(desc.size.z)), physx::PxQuat::createIdentity());
			geom = new physx::PxConvexMeshGeometry(mesh,scale);
			break;
		}
		case CPhysxColliderShapeDesc::Shape::TriangleMesh:
		{
			physx::PxDefaultMemoryInputData input(desc.cookedMeshData->data(), desc.cookedMeshData->size());
			physx::PxTriangleMesh *mesh = m_PhysX->createTriangleMesh(input);
			physx::PxMeshScale scale(physx::PxVec3(mathUtils::Abs(desc.size.x), mathUtils::Abs(desc.size.y), mathUtils::Abs(desc.size.z)), physx::PxQuat::createIdentity());
			geom = new physx::PxTriangleMeshGeometry(mesh, scale);
			break;
		}

		default:
			throw std::logic_error("Not yet implemented");
			break;
	}

	physx::PxTransform actorTransform = physx::PxTransform(v(desc.position), q(desc.orientation));

	physx::PxRigidActor* body;
	switch (actorType)
	{
		case CPhysXManager::ActorType::Static:
			body = m_PhysX->createRigidStatic(actorTransform);
			break;
		case CPhysXManager::ActorType::Dynamic:
			body = m_PhysX->createRigidDynamic(actorTransform);
			if (isKinematic)
			{
				(static_cast<physx::PxRigidBody*>(body))->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
			}
			break;
	}

	physx::PxShape* shape = body->createShape(*geom, *mat);
	delete geom;

	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, 0, 0), physx::PxQuat::createIdentity());

	shape->setLocalPose(transform);

	body->userData = reinterpret_cast<void*>(idx);
	if (actorType == ActorType::Dynamic)
	{
		physx::PxRigidBodyExt::updateMassAndInertia(*static_cast<physx::PxRigidBody*>(body), desc.density);
	}
	m_Scene->addActor(*body);

	DEBUG_ASSERT(m_actors.actor.size() == m_actors.index.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.name.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.position.size());
	DEBUG_ASSERT(m_actors.actor.size() == m_actors.rotation.size());

	m_actors.index[name] = idx;
	m_actors.name.push_back(name);
	m_actors.position.push_back(desc.position);
	m_actors.rotation.push_back(desc.orientation);
	m_actors.actor.push_back(body);
}

void CPhysXManager::MoveActor(std::string name, Vect3f position, Quatf rotation)
{
	auto it = m_actors.index.find(name);
	if (it != m_actors.index.end())
	{
		size_t id = it->second;
		physx::PxRigidActor* rigid = static_cast<physx::PxRigidActor*>(m_actors.actor[id]);
		rigid->setGlobalPose(physx::PxTransform(v(position), q(rotation)));
	}
}

void CPhysXManager::createController(float height, float radius, float density, Vect3f pos, std::string name){
	physx::PxMaterial* l_material = m_materials["controller_material"];
	physx::PxCapsuleControllerDesc desc;
	//Skin predefined contact offset 0.1f
	desc.contactOffset = 0.01f;
	desc.height = height;
	desc.radius = radius;
	desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
	desc.slopeLimit = cosf(3.1415f / 6); //30º
	desc.stepOffset = 0.5f;
	desc.density = density;
	desc.reportCallback = NULL; //TODO
	desc.position = physx::PxExtendedVec3(pos.x, pos.y + height*0.5f + radius + desc.contactOffset, pos.z);
	desc.material = l_material;
	int index = m_CharacterControllers.size();
	desc.userData = (void*) index;
	physx::PxController* cct = m_ControllerManager->createController(desc);
	m_CharacterControllers[name] = cct;
}

void CPhysXManager::InitPhysx(){
	registerMaterial("ground", 1, 0.9, 0.1);
	registerMaterial("StaticObjectMaterial", 1, 0.9, 0.8);
	registerMaterial("controller_material", 10, 2, 0.5);
	createPlane("ground", "ground", Vect4f(0, 1, 0, 0));
}


Vect3f CPhysXManager::moveCharacterController(Vect3f displacement, Vect3f up, float elapsedTime, const std::string &name)
{
	physx::PxController* cct = getCharControllers()[name];
	const physx::PxControllerFilters filters(nullptr, nullptr, nullptr);
	size_t index = (size_t)cct->getUserData();
	physx::PxRigidDynamic* actor = cct->getActor();
	cct->move(v(displacement), 0.0001, elapsedTime, filters);
	cct->setUpDirection(v(up));
	//physx::PxExtendedVec3 pFootPos = cct->getFootPosition();
	//physx::PxVec3 vel = actor->getLinearVelocity();
	return v(cct->getFootPosition());
}

bool CPhysXManager::isCharacterControllerGrounded( const std::string &name )
{
	physx::PxController* cct = getCharControllers()[name];

	physx::PxControllerState state;
	cct->getState( state );
	return state.touchedActor != nullptr;
}

void CPhysXManager::releaseCharacterController( const std::string &name )
{
	auto it = m_CharacterControllers.find( name );
	if ( it == m_CharacterControllers.end() )
	{
		DEBUG_ASSERT( it == m_CharacterControllers.end() );
		return;
	}
	physx::PxController *cct = it->second;
	cct->release();
	m_CharacterControllers.erase( it );
}

void CPhysXManager::releaseCharacterControllers(){
	for (auto it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); it++){
		physx::PxController *cct = it->second;
		cct->release();
	}
	m_CharacterControllers.clear();
}

void CPhysXManager::update(float dt)
{
	m_elapsedTime += dt;
	if (m_elapsedTime >= PHYSX_UPDATE_STEP)
	{
		m_Scene->simulate(PHYSX_UPDATE_STEP);
		m_Scene->fetchResults(true);
		m_elapsedTime = fmod(m_elapsedTime, PHYSX_UPDATE_STEP);

		// Update actor positions
		physx::PxU32 nActiveTransf;
		auto activeTransf = m_Scene->getActiveTransforms(nActiveTransf);
		for (physx::PxU32 i = 0; i < nActiveTransf; ++i)
		{
			size_t idx = reinterpret_cast<size_t>(activeTransf[i].userData);
			m_actors.position[idx] = v(activeTransf[i].actor2World.p);
			m_actors.rotation[idx] = q(activeTransf[i].actor2World.q);
		}
	}
}

