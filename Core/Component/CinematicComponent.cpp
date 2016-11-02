#include "CinematicComponent.h"

#include "Scene/Element.h"
#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Graphics\Cinematics\CinematicManager.h>
#include "ComponentManager.h"

const std::string CCinematicComponent::COMPONENT_TYPE = "Cinematic";

CCinematicComponent::CCinematicComponent(CXMLTreeNode& node, CElement* Owner)
	: CComponent(node, Owner)
{
	SetNameFromParentName( Owner->getName() );

	bool playAtStart = node.GetBoolProperty("playAtStart", false);
	if (playAtStart)
	{
		CEngine::GetSingleton().getCinematicManager()->Play(node.GetPszISOProperty("name","none"));
	}
}

CCinematicComponent::CCinematicComponent(const CCinematicComponent& base, CElement* Owner)
	: CComponent(base, Owner)
{
	SetNameFromParentName( Owner->getName() );
}

CCinematicComponent::~CCinematicComponent()
{
}

void CCinematicComponent::Destroy()
{
}

void CCinematicComponent::Init()
{
}

void CCinematicComponent::FixedUpdate(double ElapsedTime)
{
}

