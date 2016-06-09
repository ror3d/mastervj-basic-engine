#include "Material.h"
#include "MaterialManager.h"
#include "Effect/EffectManager.h"

#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>

CMaterialManager::CMaterialManager()
	: m_Filename("")
{
}


CMaterialManager::~CMaterialManager()
{
}


void CMaterialManager::load(const std::string &Filename)
{
	m_Filename = Filename;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Materials = l_XML["materials"];
		if (l_Materials.Exists())
		{
			for (int i = 0; i < l_Materials.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Material = l_Materials(i);

				if (l_Material.GetName() == std::string("material"))
				{
					CMaterial * Material = new CMaterial(l_Material);
					add(Material->getName() , Material);
				}
			}
		}
	}
}


void CMaterialManager::reload()
{
	setDirty();
	load(m_Filename);
}
