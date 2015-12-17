#include "AnimatedCoreModel.h"
#include "Material/Material.h"
#include "Material/MaterialManager.h"

#include <cal3d/cal3d.h>
#include <XML/XMLTreeNode.h>

CAnimatedCoreModel::CAnimatedCoreModel(CXMLTreeNode& node)
	: CNamed(node)
	, m_CalCoreModel(nullptr)
{
	m_Path = node.GetPszISOProperty("path");
	m_CalCoreModel = new CalCoreModel(getName());
	Load();
}
CAnimatedCoreModel::~CAnimatedCoreModel()
{
	if (m_CalCoreModel)
	{
		delete m_CalCoreModel;
	}
	for (auto it = m_Materials.begin(); it != m_Materials.end(); ++it)
	{
		delete *it;
	}
	m_Materials.clear();
}

bool CAnimatedCoreModel::LoadMesh(CXMLTreeNode& treeNode)
{
	std::string fname = m_Path + treeNode.GetPszProperty("filename");
	if (m_CalCoreModel->loadCoreMesh(fname) < 0)
	{
		return false;
	}
	return true;
}

bool CAnimatedCoreModel::LoadSkeleton(CXMLTreeNode& treeNode)
{
	std::string fname = m_Path + treeNode.GetPszProperty("filename");
	if (m_CalCoreModel->loadCoreSkeleton(fname) < 0)
	{
		return false;
	}
	return true;
}
bool CAnimatedCoreModel::LoadAnimation(CXMLTreeNode& treeNode)
{
	std::string fname = m_Path + treeNode.GetPszProperty("filename");
	if (m_CalCoreModel->loadCoreAnimation(fname) < 0)
	{
		return false;
	}
	return true;
}


CalCoreModel * CAnimatedCoreModel::GetCoreModel() const
{
	return m_CalCoreModel;
}

void CAnimatedCoreModel::Load()
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile((m_Path+"actor.xml").c_str()))
	{
		CXMLTreeNode actor = l_XML["actor"];
		if (actor.Exists())
		{
			for (int i = 0; i < actor.GetNumChildren(); ++i)
			{
				CXMLTreeNode node = actor(i);
				if (std::string(node.GetName()) == "skeleton")
				{
					LoadSkeleton(node);
				}
				else if (std::string(node.GetName()) == "mesh")
				{
					LoadMesh(node);
				}
				else if (std::string(node.GetName()) == "material")
				{
					m_Materials.push_back(new CMaterial(node));
				}
				else if (std::string(node.GetName()) == "animation")
				{
					LoadAnimation(node);
				}
			}
		}
	}
}