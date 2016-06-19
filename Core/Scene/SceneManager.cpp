#include "SceneManager.h"
#include "Scene.h"
#include "Element.h"

#include <Windows.h>

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
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
			CScene * scene = new CScene();
			scene->Load(fname, this);
			add(scene->getName(), scene);
		}
	} while ( FindNextFile( hFind, &ffd ) != 0 );

	FindClose( hFind );
}

void CSceneManager::EnableScene( const std::string& scene )
{
}

void CSceneManager::DisableScene( const std::string& scene )
{
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
		delete it->second;
		m_Objects.erase( it );
	}
}

CElement * CSceneManager::GetObjectById( const std::string & id )
{
	auto it = m_Objects.find( id );

	if ( it != m_Objects.end() )
	{
		return it->second;
	}

	return nullptr;
}

