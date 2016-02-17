#include "MeshInstance.h"
#include <Engine/Engine.h>
#include "StaticMesh.h"

CMeshInstance::CMeshInstance( CXMLTreeNode& treeNode )
	: CRenderableObject(treeNode)
{
	std::string CoreName = treeNode.GetPszProperty("core_name");
	m_StaticMesh = CEngine::GetSingleton().getStaticMeshManager()->get(CoreName);
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
