#include "MeshInstance.h"
#include <Engine/Engine.h>
#include "StaticMesh.h"

CMeshInstance::CMeshInstance(const std::string &Name, const std::string &CoreName)
{
	setName(Name);
    m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(CoreName);
}

CMeshInstance::~CMeshInstance()
{
	m_StaticMesh = nullptr;
}

void CMeshInstance::Render(CContextManager *_context)
{
	CEffectManager::m_Parameters.m_World=GetTransform();
	//CEngine::GetSingleton().getEffectsManager()->m_Parameters.m_World.SetFromPosAndAnglesYXZ(m_Position, m_Yaw, m_Pitch, m_Roll);;
	m_StaticMesh->Render(_context);
}