#include "Scene.h"

#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>
#include "Element.h"
#include "SceneManager.h"

CScene::CScene( const std::string& file, CSceneManager* manager )
	: m_sm(manager)
	, m_SceneFile(file)
	, m_isLoaded(false)
{
	CXMLTreeNode scene;
	scene.LoadFile(m_SceneFile.c_str());
	DEBUG_ASSERT(scene.Exists());
	if (!scene.Exists())
	{
		return;
	}

	std::string name = scene.GetPszProperty("name", "");
	DEBUG_ASSERT(name != "");
	setName(name);
}

CScene::~CScene()
{
	Unload();
}

void CScene::Load()
{
	if (m_isLoaded)
	{
		return;
	}
	CXMLTreeNode scene;
	scene.LoadFile(m_SceneFile.c_str());
	DEBUG_ASSERT(scene.Exists());
	if (!scene.Exists())
	{
		return;
	}

	for (int i = 0; i < scene.GetNumChildren(); ++i)
	{
		auto obj = scene(i);
		if (obj.GetName() != std::string("object"))
		{
			continue;
		}

		auto elem = new CElement(obj);
		m_sm->AddObject( elem );
		m_Elements.push_back(elem->getName());
	}
	m_isLoaded = true;
}

void CScene::Unload()
{
	if (!m_isLoaded)
	{
		return;
	}

	for (auto &const p : m_Elements)
	{
		m_sm->DestroyObject( p );
	}
	m_Elements.clear();

	m_isLoaded = false;
}
