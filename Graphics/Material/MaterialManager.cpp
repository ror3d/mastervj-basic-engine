#include "MaterialManager.h"

#include "XML/XMLTreeNode.h"

#include "Material.h"


CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

void CMaterialManager::load( const std::string &Filename )
{
	add( Filename, new CMaterial( Filename ) );
}

void CMaterialManager::Reload()
{
	for ( auto it = m_resources.begin(); it != m_resources.end(); ++it )
	{
		std::string filename = it->second->getName();
		it->second->destroy();
		delete it->second;
		it->second = new CMaterial( filename );
	}
}
