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
	m_Enabled = node.GetBoolProperty("enabled", false, true);
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
			component = new CCharacterControllerComponent(comp, this);
		}
		else if (type == "fps_camera")
		{
			component = new CFPSCameraComponent(comp, this);
		}
		else if (type == "collider")
		{
			component = new CPhysxComponent(comp, this);
		}
		else if (type == "trigger")
		{
			component = new CTriggerComponent(comp, this);
		}
		else if (type == "speaker")
		{
			component = new CSpeakerComponent(comp, this);
		}
		else if (type == "mesh_instance")
		{
			component = new CMeshInstanceComponent(comp, this);
		}
		else if (type == "animated_instance")
		{
			component = new CAnimatedInstanceComponent(comp, this);
		}
		else if (type == "particle_emitter")
		{
			component = new CParticleEmitterComponent(comp, this);
		}
		else
		{
			OutputDebugStringA(("Component " + type + " type not recognized\n").c_str());
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
	std::vector<std::string> toRemove;
	auto cmgr = CEngine::GetSingleton().getComponentManager();
	for (auto &const comp : m_componentContainer)
	{
		comp.second->Destroy();
	}
	for (auto &const comp : m_componentContainer)
	{
		cmgr->RemoveComponent(comp.second);
	}
	m_componentContainer.clear();
}

void CElement::SetQuat( Quatf q )
{
	m_RotationYPR = Quatf::GetEulerFromQuaternion( q );
	m_TransformChanged = true;
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
	auto comp = m_componentContainer.get(getName() + "_" + CFPSCameraComponent::COMPONENT_TYPE);
	if (comp)
	{
		return dynamic_cast<CFPSCameraComponent*>(comp);
	}
	return nullptr;
}

CSpeakerComponent* CElement::GetSpeaker()
{
	auto comp = m_componentContainer.get(getName() + "_" + CSpeakerComponent::COMPONENT_TYPE);
	if (comp)
	{
		return dynamic_cast<CSpeakerComponent*>(comp);
	}
	return nullptr;
}

CCharacterControllerComponent* CElement::GetCharacterController()
{
	auto comp = m_componentContainer.get(getName() + "_" + CCharacterControllerComponent::COMPONENT_TYPE);
	if (comp)
	{
		return dynamic_cast<CCharacterControllerComponent*>(comp);
	}
	return nullptr;
}

CPhysxComponent * CElement::GetPhysxComponent()
{
	auto comp = m_componentContainer.get(getName() + "_" + CPhysxComponent::COMPONENT_TYPE);
	if (comp)
	{
		return dynamic_cast<CPhysxComponent*>(comp);
	}
	return nullptr;
}

CTriggerComponent * CElement::GetTriggerComponent()
{
	auto comp = m_componentContainer.get(getName() + "_" + CTriggerComponent::COMPONENT_TYPE);
	if (comp)
	{
		return dynamic_cast<CTriggerComponent*>(comp);
	}
	return nullptr;
}

CAnimatedInstanceComponent * CElement::GetAnimatedInstanceComponent()
{
	auto comp = m_componentContainer.get(getName() + "_" + CAnimatedInstanceComponent::COMPONENT_TYPE);
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

		Mat44f l_RotYaw;
		l_RotYaw.SetIdentity();
		l_RotYaw.RotByAngleY(-m_RotationYPR.x);
		Mat44f l_RotPitch;
		l_RotPitch.SetIdentity();
		l_RotPitch.RotByAngleX(-m_RotationYPR.y);
		Mat44f l_RotRoll;
		l_RotRoll.SetIdentity();
		l_RotRoll.RotByAngleZ(-m_RotationYPR.z);

		rotation = l_RotRoll*l_RotPitch*l_RotYaw;

		translation.SetFromPos( m_Position );

		scale.SetFromScale( m_Scale.x, m_Scale.y, m_Scale.z );

		m_TransformMatrix = scale * rotation * translation;

		m_TransformChanged = false;
	}
	return m_TransformMatrix;
}

template<typename T>
void CElement::SendMessage_t(const std::string msg, T arg1)
{
	for (auto &const c : m_componentContainer)
	{
		c.second->SendMsg(msg, arg1);
	}
}

void CElement::SendMsg( const std::string& message, CElement* arg1 )
{
	SendMessage_t( message, arg1 );
}

void CElement::SendMsg( const std::string& message, int arg1 )
{
	SendMessage_t( message, arg1 );
}

void CElement::SendMsg( const std::string& message, float arg1 )
{
	SendMessage_t( message, arg1 );
}

CElement* CElement::Clone( const std::string & newName )
{
	CElement* ret = new CElement(newName);

	ret->m_Scale = m_Scale;
	ret->m_Enabled = m_Enabled;
	ret->m_RotationYPR = m_RotationYPR;
	ret->m_Position = m_Position;
	ret->m_TransformMatrix = m_TransformMatrix;

	auto cmgr = CEngine::GetSingleton().getComponentManager();

	for ( auto &const compBasePair : m_componentContainer )
	{
		CComponent* comp = compBasePair.second->Clone( ret );
		ret->AddComponent( comp->getName(), comp );
		cmgr->AddComponent(comp);
	}
	for ( auto &const comp : ret->m_componentContainer )
	{
		comp.second->ObjectInitialized();
	}

	return ret;
}

