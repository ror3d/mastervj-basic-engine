#include "TriggerManager.h"
#include "XML/XMLTreeNode.h"
#include "Trigger.h"
#include <Core/Engine/Engine.h>
#include <Math/Math.h>
#include <PhysX/PhysXManager.h>

CTriggerManager::CTriggerManager()
{
}

bool CTriggerManager::Load(std::string fileName)
{
	m_FileName = fileName;
	CXMLTreeNode l_triggers;
	l_triggers.LoadFile(fileName.c_str());

	if (!l_triggers.Exists())
	{
		return false;
	}
	else
	{
		for (int i = 0; i < l_triggers.GetNumChildren(); ++i)
		{
			CXMLTreeNode treeNode = l_triggers(i);
			std::string name = treeNode.GetPszProperty("name");
			std::string typeTrigger = treeNode.GetPszProperty("typeTrigger");
			Vect3f scale;
			scale = treeNode.GetVect3fProperty("scale", scale);
			Vect3f pos;
			pos = treeNode.GetVect3fProperty("pos", pos);
			Vect3f rot;
			rot = treeNode.GetVect3fProperty("rotation", rot);
			rot.x = mathUtils::Deg2Rad(-rot.x);
			rot.y = mathUtils::Deg2Rad(rot.y);
			rot.z = mathUtils::Deg2Rad(-rot.z);	//From MeshInstance: Quatf::GetQuaternionFromRadians(Vect3f(-GetYaw(), GetPitch(), -GetRoll()));
			
			if (typeTrigger == "Sphere")
			{
				CEngine::GetSingleton().getPhysXManager()->createStaticSphere(
					name, scale, "StaticObjectMaterial", pos, Quatf::GetQuaternionFromRadians(rot), true);
			}
			else if (typeTrigger == "Box")
			{
				CEngine::GetSingleton().getPhysXManager()->createStaticBox(
					name, scale, "StaticObjectMaterial", pos, Quatf::GetQuaternionFromRadians(rot), true);
			}
			add(name, new CTrigger(treeNode));
		}
	}

}

void CTriggerManager::Activate(std::string name)
{
	get(name)->Activate();
}

CTriggerManager::~CTriggerManager()
{
}
