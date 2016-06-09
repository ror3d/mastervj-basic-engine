#include "MeshInstance.h"
#include <Engine/Engine.h>
#include "Mesh/StaticMesh.h"
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Core/Component/PhysxComponent.h>
#include <PhysX/PhysXManager.h>
#include <Base/XML/XMLTreeNode.h>


CMeshInstance::CMeshInstance( CXMLTreeNode& treeNode )
	: CRenderableObject(treeNode)
{
	std::string nameCore = treeNode.GetPszProperty("core_name");
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(nameCore);
}

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
	if ( m_Visible )
	{
		m_StaticMesh->Render( _context );
	}
}
