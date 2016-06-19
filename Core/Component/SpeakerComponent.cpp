#include "SpeakerComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Sound/SoundManager.h>

//const C3DElement* _speaker;
CSoundManager SoundManager;


CSpeakerComponent::CSpeakerComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	setName(name);
	l_Position = node.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f));
	l_Orientation = node.GetVect3fProperty("orientation", Vect3f(0.0f, 0.0f, 0.0f));
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
	// TODO: Crear estructures necessàries en el manager d'audio utilitzant getName com a identificador
	std::string name = getName();
	SoundManager.LoadSpeakers(name, l_Position, l_Orientation);
	l_speaker = {};
	l_speaker.SetPosition(l_Position);
	l_speaker.SetYawPitchRoll(l_Orientation.x, l_Orientation.y, l_Orientation.z);
	SoundManager.RegisterSpeaker(&l_speaker);

}


void CSpeakerComponent::Destroy()
{
	// TODO: Eliminar estructures que calgui del manager d'audio
	SoundManager.UnregisterSpeaker(&l_speaker);
}

void CSpeakerComponent::Play( const std::string EventName, bool loop )
{
	// TODO: Reproduir one-shot o bucle depenent de loop
	SoundManager.PlayEvent(EventName, &l_speaker);
}

void CSpeakerComponent::Stop()
{
	// Parar reproducció d'audios iniciats per aquest component
	SoundManager.PlayEvent((std::string)"Stop", &l_speaker);
}

void CSpeakerComponent::Update(float ElapsedTime)
{
	// TODO: Actualitzar posició i rotació del speaker en el manager d'audio a partir del GetOwner()->GetPosition() i GetOwner()->GetDirection()
	
	Vect3f newPosition = GetOwner()->GetPosition();
	Vect3f newOrientation = GetOwner()->GetRotation();
	if ((newPosition != l_Position) || (newOrientation != l_Orientation))
	{
		SoundManager.UnregisterSpeaker(&l_speaker);

		if (newPosition != l_Position)
		{
			l_speaker.SetPosition(newPosition);
		}
		if (newOrientation != l_Orientation)
		{
			l_speaker.SetYawPitchRoll(l_Orientation.x, l_Orientation.y, l_Orientation.z);
		}
		SoundManager.RegisterSpeaker(&l_speaker);
	}

}

