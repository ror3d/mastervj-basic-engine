#include "AnimatedModelManager.h"

#include "AnimatedCoreModel.h"

#include <XML/XMLTreeNode.h>

CAnimatedModelManager::CAnimatedModelManager()
{

}
CAnimatedModelManager::~CAnimatedModelManager()
{

}

void CAnimatedModelManager::Load(const std::string &Filename)
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

					auto inst = new CAnimatedCoreModel(animModel);

					add(inst->getName(), inst);
				}
			}
		}
	}

}
void CAnimatedModelManager::Reload()
{
	destroy();
	Load(CAnimatedModelManager::m_Filename);
}
