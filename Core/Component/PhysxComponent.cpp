#include "PhysxComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Mesh/StaticMesh.h>
#include <PhysX/PhysXManager.h>
#include <Core/Engine/Engine.h>

CPhysxComponent::CPhysxComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);
	
	m_colType = node.GetPszProperty("collider_type", "Sphere");
	m_isStatic = node.GetBoolProperty("static");	
	m_isKinematic = node.GetBoolProperty("kinematic");
	m_coreName = node.GetPszProperty("core_mesh", "");	
	if (m_coreName == "")
	{
		m_colType = "Sphere";
	}
	m_isTrigger = node.GetBoolProperty("trigger", false);
}

CPhysxComponent::CPhysxComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CPhysxComponent::~CPhysxComponent()
{
}

void CPhysxComponent::Init()
{
	CPhysxColliderShapeDesc desc;
	desc.material = std::string("StaticObjectMaterial");// TODO get from file
	desc.size = GetOwner()->GetScale();
	if (m_isTrigger)
	{
		Vect3f sizeS = desc.size;
		sizeS.y *= 5.0f;
		sizeS.x *= 1.2f;
		sizeS.z *= 1.2f;
		desc.size = sizeS;
	}
	desc.position = GetOwner()->GetPosition();
	desc.orientation = Quatf::GetQuaternionFromRadians(Vect3f(-GetOwner()->GetYaw(), GetOwner()->GetPitch(), -GetOwner()->GetRoll()));

	CPhysXManager::ActorType actorType;
	if (m_isStatic)
	{
		actorType = CPhysXManager::ActorType::Static;
	}
	else
	{
		actorType = CPhysXManager::ActorType::Dynamic;
	}

	CStaticMesh * m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(m_coreName);

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
		if (m_coreName != "")
		{
			//TODO: get radius
		}
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
	CEngine::GetSingleton().getPhysXManager()->createActor(getName(), actorType, desc, m_isKinematic, m_isTrigger);
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
