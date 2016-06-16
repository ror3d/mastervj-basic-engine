#include "PhysxComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Mesh/StaticMeshLoader.h>
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
	//m_isTrigger = node.GetBoolProperty("trigger", false);
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
	Init(GetOwner()->GetScale(), GetOwner()->GetPosition());
}

void CPhysxComponent::Init(Vect3f scale, Vect3f position)
{
	CPhysxColliderShapeDesc desc;
	desc.material = std::string("StaticObjectMaterial");// TODO get from file
	desc.size = scale;
	desc.position = position;
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
		bool success = CEngine::GetSingleton().getMeshLoader()->FillColliderDescriptor(m_coreName, &desc);
	}
	else // m_colType == std::string("Convex Mesh") or unrecognized type
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;
		bool success = CEngine::GetSingleton().getMeshLoader()->FillColliderDescriptor(m_coreName, &desc);
		actorType = CPhysXManager::ActorType::Static;
	}
	CEngine::GetSingleton().getPhysXManager()->createActor(getName(), actorType, desc, m_isKinematic, m_isTrigger);
}

void CPhysxComponent::Destroy()
{
	// TODO!
	//CEngine::GetSingleton().getPhysXManager()->releaseCharacterController(getName());
}

void CPhysxComponent::FixedUpdate(float ElapsedTime)
{
	if (!m_isStatic && m_isKinematic)
	{
		Move(GetOwner()->GetPosition());
	}
}

void CPhysxComponent::Move(Vect3f position)
{
	Quatf quat = Quatf::GetQuaternionFromRadians(Vect3f(-GetOwner()->GetYaw(), GetOwner()->GetPitch(), -GetOwner()->GetRoll()));
	CEngine::GetSingleton().getPhysXManager()->MoveActor(getName(), position, quat);
}
