#include "CookedMeshmanager.h"

#include <Engine/Engine.h>

#include "StaticMesh.h"
CCookedMeshManager::CCookedMeshManager()
{
}
CCookedMeshManager::~CCookedMeshManager()
{
}
void CCookedMeshManager::CookMeshes()
{
	std::map<std::string, CStaticMesh*> staticMeshes = CEngine::GetSingleton().getStaticMeshManager()->GetResources();

	for (auto it = staticMeshes.begin(); it != staticMeshes.end(); it++){
		CPhysxColliderShapeDesc desc;
		desc.shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;
		it->second->FillColliderDescriptor(&desc);

		/*shapeDesc->material = std::string("StaticObjectMaterial");
	CEngine::GetSingleton().getPhysXManager()->createActor(getName(), CPhysXManager::ActorType::Static, *shapeDesc);*/
	}

}