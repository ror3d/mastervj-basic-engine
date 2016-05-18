#include "PhysxComponent.h"

#include <Graphics/Renderable/RenderableObject.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Mesh/StaticMesh.h>
#include <PhysX/PhysXManager.h>
#include <Core/Engine/Engine.h>

CPhysxComponent::CPhysxComponent(CXMLTreeNode& node, CRenderableObject* Owner, std::string nameCore)
	: CComponent(node, Owner)
{
	setName(Owner->getName() + "_PhysxComponent");
	
	m_colType = node.GetPszProperty("collider_type");
	m_isStatic = node.GetBoolProperty("static");	
	m_isKinematic = node.GetPszProperty("kinematic");
	
	CPhysxColliderShapeDesc desc;
	desc.material = std::string("StaticObjectMaterial");// TODO get from file
	desc.size = Owner->GetScale();
	desc.position = Owner->GetPosition();
	desc.orientation = Quatf::GetQuaternionFromRadians(Vect3f(-Owner->GetYaw(), Owner->GetPitch(), -Owner->GetRoll()));

	CPhysXManager::ActorType actorType;	
	if (m_isStatic)
	{
		actorType = CPhysXManager::ActorType::Static;
	}
	else
	{		
		actorType = CPhysXManager::ActorType::Dynamic;
	}

	CStaticMesh * m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(nameCore);

	if (m_colType == std::string("Box"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::Box;
	}
	else if (m_colType == std::string("Capsule"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::Capsule;
		//TODO: get radius and halfHeight
		desc.radius = 1;
		desc.halfHeight = 1;
	}
	else if (m_colType == std::string("Sphere"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::Sphere;
		desc.radius = 1;
		//TODO: get radius
	}
	else if (m_colType == std::string("Plane"))
	{
		//TODO: Tratar como box con escala y 0.001?
		desc.size.y = 0.001f;
	}
	else if (m_colType == std::string("Triangle Mesh"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::TriangleMesh;
		m_StaticMesh->FillColliderDescriptor(&desc);
	}
	else // m_colType == std::string("Convex Mesh") or unrecognized type
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;
		m_StaticMesh->FillColliderDescriptor(&desc);
		actorType = CPhysXManager::ActorType::Static;
	}

	CEngine::GetSingleton().getPhysXManager()->createActor(getName(), actorType, desc, m_isKinematic);
}

CPhysxComponent::CPhysxComponent(CRenderableObject* Owner)
	: CComponent(Owner->getName() + "_PhysxComponent", Owner)
{
}

CPhysxComponent::~CPhysxComponent()
{
}

void CPhysxComponent::Init()
{
}

void CPhysxComponent::Destroy()
{
	//CEngine::GetSingleton().getPhysXManager()->releaseCharacterController(getName());
}

void CPhysxComponent::FixedUpdate(float ElapsedTime)
{
	if (!m_isStatic && m_isKinematic)
	{
		Move();
	}
}

void CPhysxComponent::Move()
{
	Quatf quat = Quatf::GetQuaternionFromRadians(Vect3f(-GetOwner()->GetYaw(), GetOwner()->GetPitch(), -GetOwner()->GetRoll()));
	CEngine::GetSingleton().getPhysXManager()->MoveActor(getName(), GetOwner()->GetPosition(), quat);
}
