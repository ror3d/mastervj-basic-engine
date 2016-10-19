#include "SceneManager.h"
#include "Scene.h"
#include "Element.h"

#include <Windows.h>
#include <Core\Engine\Engine.h>
#include <PhysX\PhysXManager.h>

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
	CleanupObjects();
	CleanupObjects(); // Do it twice to clear the objects to destroy buffer
	DEBUG_ASSERT( m_Objects.size() == 0 );
}

void CSceneManager::Initialize( std::string scenesDirectory )
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;

	std::string globPath;

	if ( scenesDirectory.find( '*' ) == scenesDirectory.npos )
	{
		if ( scenesDirectory.back() != '\\' && scenesDirectory.back() != '/' )
		{
			scenesDirectory += "\\";
		}
		globPath = scenesDirectory;
		globPath += "*.xml";
	}
	else
	{
		// We should NOT have a path with globs!
		DEBUG_ASSERT(false);
	}

	hFind = FindFirstFileA( globPath.c_str(), &ffd );

	DEBUG_ASSERT( INVALID_HANDLE_VALUE != hFind );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return;
	}

	do {
		if ( !( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			std::string fname = scenesDirectory + ffd.cFileName;
			CScene * scene = new CScene(fname, this);
			add(scene->getName(), scene);
		}
	} while ( FindNextFile( hFind, &ffd ) != 0 );

	FindClose( hFind );
}

void CSceneManager::LoadScene( const std::string& scene )
{
	m_ScenesToLoad.push_back(scene);
}

void CSceneManager::UnloadScene( const std::string& scene )
{
	m_ScenesToUnload.push_back(scene);
}

bool CSceneManager::IsSceneLoaded( const std::string & sceneName ) const
{
	auto sc = m_resources.find( sceneName );
	return sc != m_resources.end() && sc->second->IsLoaded();
}

bool CSceneManager::IsSceneUnloaded( const std::string & sceneName ) const
{
	auto sc = m_resources.find( sceneName );
	return sc != m_resources.end() && !sc->second->IsLoaded() && sc->second->CountElements() == 0;
}

void CSceneManager::AddObject( CElement * obj )
{
	auto it = m_Objects.find( obj->getName() );

	DEBUG_ASSERT( it == m_Objects.end() );

	m_Objects[obj->getName()] = obj;
}

void CSceneManager::DestroyObject( const std::string & id )
{
	auto it = m_Objects.find( id );

	DEBUG_ASSERT( it != m_Objects.end() );

	if ( it != m_Objects.end() )
	{
		m_ObjectsToDestroy.push_back( id );
		it->second->Destroy();
	}
}

void CSceneManager::CleanupObjects()
{
	for ( auto &id : m_ObjectsReadyToDestroy )
	{
		auto it = m_Objects.find( id );

		if ( it != m_Objects.end() )
		{
			it->second->GetScene()->RemoveObject( id );
			delete it->second;
			m_Objects.erase( it );
		}
	}

	m_ObjectsReadyToDestroy = std::move(m_ObjectsToDestroy);

	m_ObjectsToDestroy.clear();
}

void CSceneManager::AddObjectToScene( const std::string& sceneName, CElement* obj )
{
	CScene* scene = get( sceneName );

	DEBUG_ASSERT( scene != nullptr );

	if ( scene != nullptr )
	{
		AddObject( obj );
		scene->AddObject( obj->getName() );
	}
}

void CSceneManager::DestroyObjectFromScene( const std::string & sceneName, const std::string & objName )
{
	CScene* scene = get( sceneName );

	DEBUG_ASSERT( scene != nullptr );

	if ( scene != nullptr )
	{
		scene->RemoveObject( objName );
		DestroyObject( objName );
	}
}

CElement * CSceneManager::GetObjectById( const std::string & id )
{
	auto it = m_Objects.find( id );
	auto it2 = std::find( m_ObjectsToDestroy.begin(), m_ObjectsToDestroy.end(), id );

	if ( it != m_Objects.end() && it2 == m_ObjectsToDestroy.end())
	{
		return it->second;
	}

	return nullptr;
}

void CSceneManager::FixedUpdate()
{
	CleanupObjects();
}

void CSceneManager::Update()
{
	std::vector<std::string> scenesNotYetLoaded;

	for (auto &const scene : m_ScenesToLoad)
	{
		auto sc = get(scene);
		if (sc)
		{
			if(!sc->IsLoaded() && sc->CountElements() == 0)
			{
				sc->Load();
			}
			else if( sc->CountElements() > 0 // If scene started being unloaded but objects are still present
					 || std::find(m_ScenesToUnload.begin(), m_ScenesToUnload.end(), scene) != m_ScenesToLoad.end()) // If we are about to start unloading the scene
			{
				scenesNotYetLoaded.push_back(scene);
			}
			// Else we just skip it since we aren't unloading it currently
		}
	}

	m_ScenesToLoad.swap( scenesNotYetLoaded );

	for (auto &const scene : m_ScenesToUnload)
	{
		auto sc = get(scene);
		if (sc)
		{
			sc->Unload();
		}
	}

	m_ScenesToUnload.clear();
}

void CSceneManager::StartedUnload()
{
	CEngine::GetSingleton().getPhysXManager()->m_enabledTriggerDetection = false;
}

void CSceneManager::FinishedLoad()
{
	CEngine::GetSingleton().getPhysXManager()->m_enabledTriggerDetection = true;
}

