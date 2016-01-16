#include "PhysXManager.h"

#include <PxPhysicsAPI.h>

#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysXProfileSDKDEBUG.lib")
#pragma comment(lib, "PhysX3CharacterKinematicDEBUG_x86.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x86")

/* // Not needed
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

