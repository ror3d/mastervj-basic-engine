#include "SpeakerComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Sound/SoundManager.h>

//const C3DElement* _speaker;


CSpeakerComponent::CSpeakerComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);
	l_Position = Owner->GetPosition();
	l_Orientation.x = Owner->GetYaw();
	l_Orientation.y = Owner->GetPitch();
	l_Orientation.z = Owner->GetRoll();
}

CSpeakerComponent::CSpeakerComponent(const std::string& name, CElement* Owner)
	: CComponent(name, Owner)
{
}

CSpeakerComponent::~CSpeakerComponent()
{
}

void CSpeakerComponent::Init()
{
	//Crear estructures necessàries en el manager d'audio utilitzant getName com a identificador
	//SoundManager.Init();
	std::string name = getName();
	CEngine::GetSingleton().getSoundManager()->LoadSpeakers(name, l_Position, l_Orientation);
	l_speaker = {};
	l_speaker.SetPosition(l_Position);
	l_speaker.SetYawPitchRoll(l_Orientation.x, l_Orientation.y, l_Orientation.z);
	CEngine::GetSingleton().getSoundManager()->RegisterSpeaker(&l_speaker);
	//Play((std::string)"Play", true);

}


void CSpeakerComponent::Destroy()
{
	// Eliminar estructures que calgui del manager d'audio
	CEngine::GetSingleton().getSoundManager()->UnregisterSpeaker(&l_speaker);
}

void CSpeakerComponent::Play( const std::string EventName, bool loop )
{
	// TODO: Reproduir one-shot o bucle depenent de loop
	C3DElement nspeaker = {};
	CEngine::GetSingleton().getSoundManager()->PlayEvent(EventName, &l_speaker);

}

void CSpeakerComponent::Stop()
{
	// Parar reproducció d'audios iniciats per aquest component
	CEngine::GetSingleton().getSoundManager()->PlayEvent((std::string)"Stop", &l_speaker);
}

void CSpeakerComponent::Update(float ElapsedTime)
{
	// TODO: Actualitzar posició i rotació del speaker en el manager d'audio a partir del GetOwner()->GetPosition() i GetOwner()->GetDirection()
	
	Vect3f newPosition = GetOwner()->GetPosition();
	Vect3f newOrientation = GetOwner()->GetRotation();
	if ((newPosition != l_Position) || (newOrientation != l_Orientation))
	{
		CEngine::GetSingleton().getSoundManager()->UnregisterSpeaker(&l_speaker);

		if (newPosition != l_Position)
		{
			l_Position = newPosition;
			l_speaker.SetPosition(l_Position);
		}
		if (newOrientation != l_Orientation)
		{
			l_Orientation = newOrientation;
			l_speaker.SetYawPitchRoll(l_Orientation.x, l_Orientation.y, l_Orientation.z);
		}
		CEngine::GetSingleton().getSoundManager()->RegisterSpeaker(&l_speaker);
	}

}

