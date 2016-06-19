#include "AnimatedMeshManager.h"

#include "AnimatedMesh.h"

#include <XML/XMLTreeNode.h>

#include "cal3d/loader.h"

CAnimatedMeshManager::CAnimatedMeshManager()
{
	CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
}
CAnimatedMeshManager::~CAnimatedMeshManager()
{

}

void CAnimatedMeshManager::Load(const std::string &Filename)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode animatedModels = l_XML["animated_models"];
		if (animatedModels.Exists())
		{
			for (int i = 0; i < animatedModels.GetNumChildren(); ++i)
			{
				CXMLTreeNode animated_model = animatedModels(i);

				if (animated_model.GetName() == std::string("animated_model"))
				{
					CXMLTreeNode &animModel = animated_model;

					auto inst = new CAnimatedMesh(animModel);

					add(inst->getName(), inst);
				}
			}
		}
	}

}
void CAnimatedMeshManager::Reload()
{
	destroy();
	Load(CAnimatedMeshManager::m_Filename);
}
