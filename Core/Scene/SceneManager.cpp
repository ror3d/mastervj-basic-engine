#include "SceneManager.h"
#include "Scene.h"

#include <Windows.h>


void CSceneManager::Initialize( std::string scenesDirectory )
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;

	if ( scenesDirectory.find( '*' ) == scenesDirectory.npos )
	{
		if ( scenesDirectory.back() != '\\' && scenesDirectory.back() != '/' )
		{
			scenesDirectory += "\\";
		}
		scenesDirectory += "*.xml";
	}

	hFind = FindFirstFileA( scenesDirectory.c_str(), &ffd );

	DEBUG_ASSERT( INVALID_HANDLE_VALUE != hFind );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return;
	}

	do {
		if ( !( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			std::string fname = ffd.cFileName;
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

