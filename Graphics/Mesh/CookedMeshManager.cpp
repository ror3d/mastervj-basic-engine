#include "CookedMeshmanager.h"

#include <Engine/Engine.h>
#include <PhysX/PhysXManager.h>

#include <sstream>

CCookedMeshManager::CCookedMeshManager()
{
}
CCookedMeshManager::~CCookedMeshManager()
{
}

std::string CCookedMeshManager::GetValidName( const std::string path )
{
	std::stringstream ss;
	for ( int i = 0; i < path.size(); ++i )
	{
		char c = path[i];
		if ( ( c >= 'A' && c <= 'Z' )
			|| ( c >= 'a' && c <= 'z' )
			|| ( c >= '0' && c <= '9' )
			|| c == '_' || c == '+' )
		{
			ss << path[i];
		}
		else if ( c == '-' )
		{
			ss << '_';
		}
		else
		{
			ss << '-';
		}
	}
	return ss.str();
}

bool CCookedMeshManager::Load( const std::string& meshName, std::vector<uint8> * &cooked )
{
	if ( m_resources.find( meshName ) != m_resources.end() )
		{
		cooked = m_resources[meshName];
		return true;
		}
	std::string nameFile = m_cookedMeshesPath;
	nameFile +=  GetValidName(meshName) + ".bin";
	cooked = new std::vector<uint8>;
	bool ret = CEngine::GetSingleton().getPhysXManager()->loadCookedMesh( nameFile, *cooked );
	if ( ret )
		{
		MapManagerType_t::add( meshName, cooked );
		}
	return ret;
	}

void CCookedMeshManager::add( const std::string& meshName, std::vector<uint8>* cooked )
{
	std::string nameFile = m_cookedMeshesPath;
	nameFile +=  GetValidName(meshName) + ".bin";

	CEngine::GetSingleton().getPhysXManager()->saveCookedMeshToFile(*cooked, nameFile);

	MapManagerType_t::add( meshName, cooked );
}


