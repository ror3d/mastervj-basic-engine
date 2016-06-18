#include "Element.h"

#include <Base/XML/XMLTreeNode.h>

#include <Core/Engine/Engine.h>

#include <Core/Component/ComponentManager.h>
#include <Core/Component/ScriptedComponent.h>
#include <Core/Component/CharControllerComponent.h>
#include <Core/Component/FPSCameraComponent.h>
#include <Core/Component/PhysxComponent.h>
#include <Core/Component/TriggerComponent.h>
#include <Core/Component/MeshInstanceComponent.h>
#include <Core/Component/AnimatedInstanceComponent.h>
#include <Core/Component/ParticleEmitterComponent.h>
#include <Core/Component/AnimatedInstanceComponent.h>
#include <Core/Component/SpeakerComponent.h>

#include <Windows.h>

CElement::CElement()
	: m_Scale(1.0f, 1.0f, 1.0f)
	, m_Enabled(true)
	, m_TransformChanged(true)
{}

CElement::CElement(const std::string& name)
	: CNamed(name)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_Enabled(true)
	, m_TransformChanged(true)
{
}

CElement::CElement(const CXMLTreeNode& node)
	: CNamed(node)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_Enabled(true)
	, m_TransformChanged(true)
{
	m_Position = node.GetVect3fProperty( "pos" , Vect3f(0, 0, 0), false);
	m_Scale = node.GetVect3fProperty( "scale" , Vect3f(1, 1, 1), false);
	if (node.GetPszProperty("rotationq", nullptr, false) != nullptr)
	{
		Vect4f f4 = node.GetVect4fProperty("rotationq", Vect4f(0, 0, 0, 1), true);
		auto q = Quatf(f4.x, f4.y, f4.z, f4.w);
		SetQuat( q );
	}
	else
	{
		float yaw, pitch, roll;
		if (node.GetPszProperty("rotation", nullptr, false) != nullptr)
		{
			Vect3f rot = node.GetVect3fProperty("rotation", Vect3f(0, 0, 0), false);
			yaw = rot.x;
			pitch = rot.y;
			roll = rot.z;
		}
		else
		{
			yaw = node.GetFloatProperty("yaw");
			pitch = node.GetFloatProperty("pitch");
			roll = node.GetFloatProperty("roll");
		}
		m_RotationYPR.x = mathUtils::Deg2Rad(yaw);
		m_RotationYPR.y = mathUtils::Deg2Rad(pitch);
		m_RotationYPR.z = mathUtils::Deg2Rad(roll);
	}


	auto cmgr = CEngine::GetSingleton().getComponentManager();
	for (int i = 0; i < node.GetNumChildren(); ++i)
	{
		auto comp = node(i);
		std::string type = comp.GetName();
		CComponent* component;
		if (type == "scripted_component")
		{
			std::string name = comp.GetPszProperty("class");
			component = new CScriptedComponent(comp, this);
		}
		else if (type == "character_controller")
		{
			component = new CCharacterControllerComponent(getName() + "_CharacterController", comp, this);
		}
		else if (type == "fps_camera")
		{
			component = new CFPSCameraComponent(getName() + "_Camera", comp, this);
		}
		else if (type == "collider")
		{
			component = new CPhysxComponent(getName() + "_Collider", comp, this);
		}
		else if (type == "trigger")
		{
			component = new CTriggerComponent(getName() + "_Trigger", comp, this);
		}
		else if (type == "speaker")
		{
			component = new CSpeakerComponent(getName() + "_Speaker", comp, this);
		}
		else if (type == "mesh_instance")
		{
			component = new CMeshInstanceComponent(getName() + "_MeshInstance", comp, this);
		}
		else if (type == "animated_instance")
		{
			component = new CAnimatedInstanceComponent(getName() + "_AnimatedInstance", comp, this);
		}
		else if (type == "particle_emitter")
		{
			component = new CParticleEmitterComponent(getName() + "_ParticleEmitter", comp, this);
		}
		else
		{
			OutputDebugStringA("Component type not recognized\n");
			continue;
		}
		cmgr->AddComponent(component);
		AddComponent(component->getName(), component);
	}
	for ( auto const& comp : m_componentContainer )
	{
		comp.second->ObjectInitialized();
	}
}


CElement::~CElement()
{
	auto cmgr = CEngine::GetSingleton().getComponentManager();
	for (auto &const comp : m_componentContainer)
	{
		comp.second->Destroy();
	}
	for (auto &const comp : m_componentContainer)
	{
		cmgr->RemoveComponent(comp.second);
	}
	m_componentContainer.destroy();
}

void CElement::SetQuat( Quatf q )
{
	m_RotationYPR.x = atan2(2 * (q.x*q.y + q.z*q.w), 1 - 2 * (q.y*q.y + q.z*q.z));
	m_RotationYPR.y = asin(2 * (q.x*q.z - q.w*q.y));
	m_RotationYPR.z = atan2(2 * (q.x*q.w + q.y*q.z), 1 - 2 * (q.z*q.z + q.w*q.w));
}

void CElement::AddComponent(std::string Name, CComponent* component)
{
	m_componentContainer.add(Name, component);
	component->Initialize();
}

void CElement::SendMsg(const std::string msg)
{
	for (auto it = m_componentContainer.begin(); it != m_componentContainer.end(); it++)
	{
		it->second->SendMsg(msg);
	}
}

CFPSCameraComponent* CElement::GetCamera()
{
	auto comp = m_componentContainer.get(getName() + "_Camera");
	if (comp)
	{
		return dynamic_cast<CFPSCameraComponent*>(comp);
	}
	return nullptr;
}

CSpeakerComponent* CElement::GetSpeaker()
{
	auto comp = m_componentContainer.get(getName() + "_Speaker");
	if (comp)
	{
		return dynamic_cast<CSpeakerComponent*>(comp);
	}
	return nullptr;
}

CCharacterControllerComponent* CElement::GetCharacterController()
{
	auto comp = m_componentContainer.get(getName() + "_CharacterController");
	if (comp)
	{
		return dynamic_cast<CCharacterControllerComponent*>(comp);
	}
	return nullptr;
}

CPhysxComponent * CElement::GetPhysxComponent()
{
	auto comp = m_componentContainer.get(getName() + "_Physx");
	if (comp)
	{
		return dynamic_cast<CPhysxComponent*>(comp);
	}
	return nullptr;
}

CTriggerComponent * CElement::GetTriggerComponent()
{
	auto comp = m_componentContainer.get(getName() + "_Trigger");
	if (comp)
	{
		return dynamic_cast<CTriggerComponent*>(comp);
	}
	return nullptr;
}

CAnimatedInstanceComponent * CElement::GetAnimatedInstanceComponent()
{
	auto comp = m_componentContainer.get(getName() + "_AnimatedInstance");
	if (comp)
	{
		return dynamic_cast<CAnimatedInstanceComponent*>(comp);
	}
	return nullptr;
}

const Mat44f & CElement::GetTransform()
{
	if ( m_TransformChanged )
	{
		Mat44f rotation;
		Mat44f translation;
		Mat44f scale;

		rotation.SetIdentity();
		Mat44f l_RotX;
		l_RotX.SetIdentity();
		l_RotX.RotByAngleX(m_RotationYPR.z);
		Mat44f l_RotY;
		l_RotY.SetIdentity();
		l_RotY.RotByAngleY(m_RotationYPR.x);
		Mat44f l_RotZ;
		l_RotZ.SetIdentity();
		l_RotZ.RotByAngleZ(m_RotationYPR.y);

		rotation = l_RotX*l_RotZ*l_RotY;

		translation.SetFromPos( m_Position );

		scale.SetFromScale( m_Scale.x, m_Scale.y, m_Scale.z );

		m_TransformMatrix = scale * rotation * translation;

		m_TransformChanged = false;
	}
	return m_TransformMatrix;
}

