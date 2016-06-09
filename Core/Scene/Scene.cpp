#include "Scene.h"

#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>

CScene::CScene()
{
}

void CScene::Load(const std::string& file)
{
	CXMLTreeNode scene;
	scene.LoadFile(file.c_str());
	DEBUG_ASSERT(scene.Exists());
	if (!scene.Exists())
	{
		return;
	}

	setName(scene.GetPszProperty("name", ""));

	for (int i = 0; i < scene.GetNumChildren(); ++i)
	{
		auto obj = scene(i);
		if (obj.GetName() != std::string("object"))
		{
			continue;
		}
	}
}
