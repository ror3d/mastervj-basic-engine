#include "CinematicManager.h"

#include "XML/XMLTreeNode.h"
#include "Engine/Engine.h"
#include "Cinematics/CinematicObject.h"
#include "Cinematics/CameraKeyController.h"

CCinematicManager::CCinematicManager()
{
}

CCinematicManager::~CCinematicManager()
{
	destroy();
}

void CCinematicManager::destroy()
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); ++it)
	{
		delete *it;
	}
	m_cinematicsObjects.clear();
}

void CCinematicManager::Load(std::string nameFile)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(nameFile.c_str()))
	{
		CXMLTreeNode l_Cinematics = l_XML["cinematic"];
		if (l_Cinematics.Exists())
		{
			for (int i = 0; i < l_Cinematics.GetNumChildren(); ++i)
			{
				if (l_Cinematics(i).GetName() == std::string("cinematic_object"))
				{
					CCinematicObject * cinematicObject = new CCinematicObject(l_Cinematics(i));
					m_cinematicsObjects.push_back(cinematicObject);
				}
				else if (l_Cinematics(i).GetName() == std::string("camera_key_controller"))
				{
					CCameraKeyController * cinematicCamera = new CCameraKeyController(l_Cinematics(i));
					CEngine::GetSingleton().getCameraManager()->add(l_Cinematics(i).GetPszProperty("name"), cinematicCamera);
					m_cinematicsCameras.push_back(cinematicCamera);
				}
			}
		}
	}
}

void CCinematicManager::Update(float _elapsedTime)
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); it++)
	{
		(*it)->Update(_elapsedTime);
	}
	for (auto it = m_cinematicsCameras.begin(); it != m_cinematicsCameras.end(); it++)
	{
		(*it)->Update(_elapsedTime);
	}


}

void CCinematicManager::Play()
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); it++)
	{
		(*it)->Play(); //TODO: cargar si anim con bucle y el nombre desde XML, para poder llamar por nombre
	}
	for (auto it = m_cinematicsCameras.begin(); it != m_cinematicsCameras.end(); it++)
	{
		(*it)->ResetTime();
		(*it)->Play(); 
	}
}
