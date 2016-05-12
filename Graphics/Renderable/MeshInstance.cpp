#include "MeshInstance.h"
#include <Engine/Engine.h>
#include "Mesh/StaticMesh.h"
#include <Graphics/Mesh/StaticMeshManager.h>
#include <PhysX/PhysXManager.h>

CMeshInstance::CMeshInstance( CXMLTreeNode& treeNode )
	: CRenderableObject(treeNode)
{
	std::string CoreName = treeNode.GetPszProperty("core_name");
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(CoreName);

	if ((CoreName == "SphereTrigger") || (CoreName == "BoxTrigger"))
	{

		if (CoreName == "SphereTrigger")
		{
			CEngine::GetSingleton().getPhysXManager()->createStaticSphere(getName(), GetScale(), "StaticObjectMaterial", GetPosition(), GetQuat(), true);
		}
		if (CoreName == "BoxTrigger")
		{
			CEngine::GetSingleton().getPhysXManager()->createStaticBox(getName(), GetScale(), "StaticObjectMaterial", GetPosition(), GetQuat(), true);
		}
	}
	else
	{
		// Create Actor
		CPhysxColliderShapeDesc desc;
		desc.shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;// TODO get from file
		//desc.shape = CPhysxColliderShapeDesc::Shape::TriangleMesh;
		m_StaticMesh->FillColliderDescriptor(&desc);

		desc.material = std::string("StaticObjectMaterial");// TODO get from file
		//desc.density = 10;// not used for static
		desc.size = GetScale();
		desc.position = GetPosition();
		desc.orientation = Quatf::GetQuaternionFromRadians(Vect3f(-GetYaw(), GetPitch(), -GetRoll()));
		CEngine::GetSingleton().getPhysXManager()->createActor(getName(), CPhysXManager::ActorType::Static, desc);
	}


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
