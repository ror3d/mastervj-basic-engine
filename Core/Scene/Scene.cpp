#include "Scene.h"

#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>
#include "Element.h"
#include "SceneManager.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	for (auto &const p : m_Elements)
	{
		m_sm->DestroyObject( p );
	}
	m_Elements.clear();
}

void CScene::Load( const std::string& file, CSceneManager* manager )
{
	m_sm = manager;

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

		auto elem = new CElement(obj);
		manager->AddObject( elem );
		m_Elements.push_back(elem->getName());
	}
}
