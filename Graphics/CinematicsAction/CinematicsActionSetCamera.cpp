#include "CinematicsActionSetCamera.h"

#include "Core/Engine/Engine.h"

CCinematicsActionSetCamera::CCinematicsActionSetCamera(CXMLTreeNode Node) : CCinematicsAction(Node)
{
	m_CameraName = Node.GetPszProperty("name", "");
}

CCinematicsActionSetCamera::~CCinematicsActionSetCamera()
{

}

void CCinematicsActionSetCamera::Execute()
{
	if (m_CameraName != "")
	{
		CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController(m_CameraName);
	}
	
}