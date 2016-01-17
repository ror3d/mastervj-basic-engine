#include "PhysXManager.h"
#include <Base/Math/Math.h>
#include <PxPhysicsAPI.h>
#include <cassert>

#if (defined(_MSC_VER) && _MSC_VER >= 1800) || __cplusplus <= 199711L
#include <thread>
#endif
#if defined(PX_WINDOWS)
#include <Windows.h>
#endif

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
#endif

#if USE_PHYSX_DEBUG
#define				PVD_HOST			"127.0.0.1"
#endif

#ifdef CHECKED_RELEASE
#undef CHECKED_RELEASE
#endif
#define CHECKED_RELEASE(x) if(x!=nullptr) { x->release(); x=nullptr; }

inline const physx::PxVec3& v(const Vect3f& v)
{ return physx::PxVec3(v.x, v.y, v.z); }
inline const Vect3f& v(const physx::PxVec3& v)
{ return Vect3f(v.x, v.y, v.z); }
inline const Vect3f& v(const physx::PxExtendedVec3& v)
{ return Vect3f((float)v.x, (float)v.y, (float)v.z); }

inline const Quatf& q(const physx::PxQuat& q)
{ return Quatf(q.x, q.y, q.z, q.w); }
inline const physx::PxQuat& q(const Quatf& q)
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

static CPhysXManager* CreatePhysXManager()
{
	return new CPhysXManagerImplementation();
}


static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

CPhysXManagerImplementation::CPhysXManagerImplementation()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	assert(m_Foundation);

	physx::PxProfileZoneManager* profileZoneManager = nullptr;
#if USE_PHYSX_DEBUG
	profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_Foundation);
#endif
	m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, profileZoneManager);
	assert(m_PhysX);

	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	assert(m_Cooking);

#if USE_PHYSX_DEBUG
	if (m_PhysX->getPvdConnectionManager())
	{
		m_PhysX->getVisualDebugger()->setVisualizeConstraints(true);
		m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		m_DebugConnection = physx::PxVisualDebuggerExt::createConnection(m_PhysX->getPvdConnectionManager(), PVD_HOST, 5425, 10);
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
	assert(m_Scene);

	m_Scene->setSimulationEventCallback(this);
	

	m_ControllerManager = PxCreateControllerManager(*m_Scene);
	m_ControllerManager->setOverlapRecoveryModule(true);


}

CPhysXManagerImplementation::~CPhysXManagerImplementation()
{
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

