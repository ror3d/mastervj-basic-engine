#include "MeshInstance.h"
#include <Engine/Engine.h>
#include "Mesh/StaticMesh.h"
#include <Graphics/Mesh/StaticMeshManager.h>
#include <PhysX/PhysXManager.h>


CMeshInstance::CMeshInstance( CXMLTreeNode& treeNode )
	: CRenderableObject(treeNode)
{
	std::string nameCore = treeNode.GetPszProperty("core_name");
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(nameCore);
	
	CPhysxColliderShapeDesc desc;

	desc.material = std::string("StaticObjectMaterial");// TODO get from file
	desc.size = GetScale();
	desc.position = GetPosition();
	desc.orientation = Quatf::GetQuaternionFromRadians(Vect3f(-GetYaw(), GetPitch(), -GetRoll()));

	std::string collisionType = treeNode.GetPszProperty("physxType","");
	if (collisionType == std::string("None"))
		return;

	if (collisionType == std::string("Box"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::Box;
	} 
	else if (collisionType == std::string("Capsule"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::Capsule;
		//TODO: get radius and halfHeight
		desc.radius = 1;
		desc.halfHeight = 1;
	}
	else if (collisionType == std::string("Sphere"))
		{
		desc.shape = CPhysxColliderShapeDesc::Shape::Sphere;
		desc.radius = 1;
		//TODO: get radius
		}
	else if (collisionType == std::string("Plane"))
		{
		//TODO: Tratar como box con escala y 0.001?
		desc.size.y = 0.001f;
		}
	else if (collisionType == std::string("Triangle Mesh"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::TriangleMesh;
		m_StaticMesh->FillColliderDescriptor(&desc);
	}
	else // collisionType == std::string("Convex Mesh") or unrecognized type
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;
		m_StaticMesh->FillColliderDescriptor(&desc);
	}

		CEngine::GetSingleton().getPhysXManager()->createActor(getName(), CPhysXManager::ActorType::Static, desc);

}

CMeshInstance::CMeshInstance(const std::string &Name, const std::string &CoreName)
{
	setName(Name);
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(CoreName);
}

CMeshInstance::~CMeshInstance()
{
	m_StaticMesh = nullptr;
}

void CMeshInstance::Render(CContextManager *_context)
{
	if ( m_Visible )
	{
		m_StaticMesh->Render( _context );
	}
}
