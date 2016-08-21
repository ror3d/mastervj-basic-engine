#include "PhysxComponent.h"

#include "Scene/Element.h"
#include "ComponentManager.h"
#include <Base/XML/XMLTreeNode.h>
#include <Graphics/Mesh/StaticMeshLoader.h>
#include <PhysX/PhysXManager.h>
#include <Core/Engine/Engine.h>

const std::string CPhysxComponent::COMPONENT_TYPE = "Collider";

CPhysxComponent::CPhysxComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
	, m_isTrigger(false)
{
	SetNameFromParentName( Owner->getName() );

	m_colType = node.GetPszProperty("collider_type", "Sphere");
	m_isStatic = node.GetBoolProperty("static", false);
	m_isKinematic = node.GetBoolProperty("kinematic", false);
	m_coreName = node.GetPszProperty("core_mesh", "");
	m_mass = node.GetFloatProperty("mass", -1.0);
	if (m_coreName == "" && (m_colType == "ConvexMesh" || m_colType == "TriangleMesh"))
	{
		DEBUG_ASSERT( !"Collider type requires core_mesh!" );
		m_colType = "Sphere";
	}
	//m_isTrigger = node.GetBoolProperty("trigger", false);
}

CPhysxComponent::CPhysxComponent(const CPhysxComponent& base, CElement* Owner)
	: CComponent(base, Owner)
	, m_isTrigger(false)
{
	SetNameFromParentName( Owner->getName() );

	m_colType = base.m_colType;
	m_isStatic = base.m_isStatic;
	m_isKinematic = base.m_isKinematic;
	m_coreName = base.m_coreName;
	m_mass = base.m_mass;
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
	desc.material = "default_material";// TODO get from file
	desc.size = scale;
	desc.position = position;
	desc.orientation = Quatf::GetQuaternionFromRadians(Vect3f(GetOwner()->GetYaw(), GetOwner()->GetPitch(), GetOwner()->GetRoll()));
	desc.mass = m_mass;
	desc.density = 1;

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
		desc.shape = CPhysxColliderShapeDesc::Shape::Box;
		desc.size.y = 0.001f;
	}
	else if (m_colType == std::string("TriangleMesh"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::TriangleMesh;
		bool success = CEngine::GetSingleton().getMeshLoader()->FillColliderDescriptor(m_coreName, &desc);
	}
	else if(m_colType == std::string("ConvexMesh"))
	{
		desc.shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;
		bool success = CEngine::GetSingleton().getMeshLoader()->FillColliderDescriptor(m_coreName, &desc);
	}
	else
	{
		DEBUG_ASSERT( !"Type not recognized!" );
	}
	CEngine::GetSingleton().getPhysXManager()->createActor(getName(), actorType, desc, m_isKinematic, m_isTrigger);
}

void CPhysxComponent::Destroy()
{
	if ( m_Destroyed ) return;
	m_Destroyed = true;
	CEngine::GetSingleton().getPhysXManager()->destroyActor(getName());
}

void CPhysxComponent::PhysxUpdate()
{
	if ( m_Destroyed ) return;
	if ( !m_isStatic )
	{
		if ( m_isKinematic )
		{
			Move( GetOwner()->GetPosition() );
		}
		else
		{
			GetOwner()->SetPosition( CEngine::GetSingleton().getPhysXManager()->getActorPosition( getName() ) );
			GetOwner()->SetQuat( CEngine::GetSingleton().getPhysXManager()->getActorRotation( getName() ) );
		}
	}
}

void CPhysxComponent::FixedUpdate(float ElapsedTime)
{
	if ( m_Destroyed ) return;
	auto collisions = CEngine::GetSingleton().getPhysXManager()->getActorCollisions( getName() );
	auto own = GetOwner();
	auto cm = CEngine::GetSingleton().getComponentManager();
	for ( auto &const col : collisions )
	{
		auto other = cm->get( col );
		if ( other )
		{
			auto otherOwner = other->GetOwner();
			own->SendMsg( "OnCollision", otherOwner );
		}
	}
}



void CPhysxComponent::Move(Vect3f position)
{
	if ( m_Destroyed ) return;
	Quatf quat = Quatf::GetQuaternionFromRadians(Vect3f(GetOwner()->GetYaw(), GetOwner()->GetPitch(), GetOwner()->GetRoll()));
	CEngine::GetSingleton().getPhysXManager()->MoveActor(getName(), position, quat);
}
