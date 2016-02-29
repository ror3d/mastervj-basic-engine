#include "CookedMeshmanager.h"
#include <Engine/Engine.h>

#include "Mesh/StaticMesh.h"
#include "Renderable/RenderableObjectsManager.h"
#include "Renderable/RenderableObject.h"
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
		CPhysxColliderShapeDesc * desc = new CPhysxColliderShapeDesc();
		desc->shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;
		it->second->FillColliderDescriptor(desc);

		desc->material = std::string("StaticObjectMaterial");
		desc->position = CEngine::GetSingleton().getLayerManager()->get("solid")->get(it->second->getName())->GetPosition();
		desc->orientation = Quatf(0, 0, 0, 1);//CEngine::GetSingleton().getLayerManager()->get("solid")->get(it->second->getName())->getQUATERNION!!
		CEngine::GetSingleton().getPhysXManager()->createActor(it->second->getName(), CPhysXManager::ActorType::Static, *desc);
	}

}