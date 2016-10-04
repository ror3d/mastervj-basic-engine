#include "CinematicManager.h"

#include "XML/XMLTreeNode.h"
#include "Engine/Engine.h"
#include "Cinematics/CinematicObject.h"
#include "Cinematics/CameraKeyController.h"
#include <Graphics/Camera/CameraManager.h>

#include <Windows.h>

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

void CCinematicManager::Remove(std::string nameToDelete)
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); it++)
	{
		if ((*it)->getName() == nameToDelete)
		{
			delete *it;
			m_cinematicsObjects.erase(it);			
			return;
		}
	}
}

void CCinematicManager::LoadFilesInDir( std::string dirPath )
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;

	std::string globPath;

	if ( dirPath.find( '*' ) == dirPath.npos )
	{
		if ( dirPath.back() != '\\' && dirPath.back() != '/' )
		{
			dirPath += "\\";
		}
		globPath = dirPath;
		globPath += "*.xml";
	}
	else
	{
		// We should NOT have a path with globs!
		DEBUG_ASSERT( false );
	}

	hFind = FindFirstFileA( globPath.c_str(), &ffd );

	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return;
	}

	do
	{
		if ( !( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			std::string fname = dirPath + ffd.cFileName;
			Load( fname );
		}
	} while ( FindNextFile( hFind, &ffd ) != 0 );

	FindClose( hFind );
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

void CCinematicManager::Play(std::string nameToRun)
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); it++)
	{
		if ((*it)->getName() == nameToRun)
		{
			(*it)->Play();
			return;
		}
	}
}


void CCinematicManager::Pause(std::string nameToRun)
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); it++)
	{
		if ((*it)->getName() == nameToRun)
		{
			(*it)->Pause();
			return;
		}
	}
}

void CCinematicManager::Stop(std::string nameToRun)
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); it++)
	{
		if ((*it)->getName() == nameToRun)
		{
			(*it)->Stop();
			return;
		}
	}
}

void CCinematicManager::Reverse(std::string nameToRun)
{
	for (auto it = m_cinematicsObjects.begin(); it != m_cinematicsObjects.end(); it++)
	{
		if ((*it)->getName() == nameToRun)
		{
			(*it)->Reverse();
			return;
		}
	}
}
