#include "SpeakerComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Sound/SoundManager.h>

//const C3DElement* _speaker;

const std::string CSpeakerComponent::COMPONENT_TYPE = "Speaker";


CSpeakerComponent::CSpeakerComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_Position = Owner->GetPosition();
	m_Position.x = -m_Position.x;
	m_Orientation.x = -Owner->GetYaw();
	m_Orientation.y = Owner->GetPitch();
	m_Orientation.z = Owner->GetRoll();
	m_Volume = node.GetFloatProperty("volume", 50.0f, false);
}

CSpeakerComponent::CSpeakerComponent(const CSpeakerComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );

	m_Position = Owner->GetPosition();
	m_Position.x = -m_Position.x;
	m_Orientation.x = Owner->GetYaw();
	m_Orientation.y = Owner->GetPitch();
	m_Orientation.z = Owner->GetRoll();
	m_Volume = base.m_Volume;
}

CSpeakerComponent::~CSpeakerComponent()
{
}

void CSpeakerComponent::Init()
{
	//Crear estructures necessàries en el manager d'audio utilitzant getName com a identificador
	//SoundManager.Init();
	std::string name = getName();
	CEngine::GetSingleton().getSoundManager()->LoadSpeakers(name, m_Position, m_Orientation);
	m_Speaker = {};
	m_Speaker.SetPosition(m_Position);
	m_Speaker.SetYawPitchRoll(m_Orientation.x, m_Orientation.y, m_Orientation.z);
	CEngine::GetSingleton().getSoundManager()->RegisterSpeaker(&m_Speaker);
	CEngine::GetSingleton().getSoundManager()->SetVolume("VolumeEffects", 0);
	//Play((std::string)"Play", true);

}


void CSpeakerComponent::Destroy()
{
	// Eliminar estructures que calgui del manager d'audio
	CEngine::GetSingleton().getSoundManager()->UnregisterSpeaker(&m_Speaker);
}

void CSpeakerComponent::Play( const std::string EventName)
{
	C3DElement nspeaker = {};
	CEngine::GetSingleton().getSoundManager()->PlayEvent(EventName, &m_Speaker);
	

}

void CSpeakerComponent::SetSwitch(const std::string SwitchValue, const std::string SwitchName)
{
	SoundSwitchValue SoundSwitch;
	SoundSwitch.m_SoundSwitch.m_SwitchName = SwitchName;
	SoundSwitch.valueName = SwitchValue;
	CEngine::GetSingleton().getSoundManager()->SetSwitch(SoundSwitch, &m_Speaker);


}

bool CSpeakerComponent::Finished(const std::string EventName)
{
	C3DElement nspeaker = {};
	AkGameObjectID id = CEngine::GetSingleton().getSoundManager()->SearchID(getName());
	bool finished = CEngine::GetSingleton().getSoundManager()->EventFinished(EventName, id);
	return finished;

}

void CSpeakerComponent::Stop()
{
	// Parar reproducció d'audios iniciats per aquest component
	CEngine::GetSingleton().getSoundManager()->PlayEvent((std::string)"Stop", &m_Speaker);
}

void CSpeakerComponent::Update(float ElapsedTime)
{
	// TODO: Actualitzar posició i rotació del speaker en el manager d'audio a partir del GetOwner()->GetPosition() i GetOwner()->GetDirection()

	Vect3f newPosition = GetOwner()->GetPosition();
	newPosition.x = -newPosition.x;
	Vect3f newOrientation = GetOwner()->GetRotation();
	if ((newPosition != m_Position) || (newOrientation != m_Orientation))
	{
		CEngine::GetSingleton().getSoundManager()->UnregisterSpeaker(&m_Speaker);

		if (newPosition != m_Position)
		{
			m_Position = newPosition;
			m_Speaker.SetPosition(m_Position);
		}
		if (newOrientation != m_Orientation)
		{
			m_Orientation = newOrientation;
			m_Speaker.SetYawPitchRoll(m_Orientation.x, m_Orientation.y, m_Orientation.z);
		}
		CEngine::GetSingleton().getSoundManager()->RegisterSpeaker(&m_Speaker);
	}

}

