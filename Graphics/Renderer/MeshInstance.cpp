#include "MeshInstance.h"

CMeshInstance::CMeshInstance(const std::string &Name, const std::string &CoreName)
	: CRenderableObject(Name)
{
    //m_StaticMesh = CEngine::GetSingletonPtr()->getStaticMeshManager()->GetResource(CoreName);

}

CMeshInstance::~CMeshInstance()
{
	m_StaticMesh = NULL;
}

void CMeshInstance::Render(CRenderManager *RM)
{
    //CEngine::GetSingletonPtr()->getEffectsManager()->SetWorldMatrix(GetTransform());
	m_StaticMesh->Render(RM);
}