#include "Scene/GenerateShadowMapsSceneRendererCommand.h"

#include <Core/Engine/Engine.h>
#include <Core/Debug/DebugPerf.h>
#include <Graphics/Renderable/RenderableObjectTechniqueManager.h>
#include <Graphics/Light/LightManager.h>
#include <Base/XML/XMLTreeNode.h>
#include "Renderable/PoolRenderableObjectTechnique.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CSceneRendererCommand(TreeNode)
	, m_pool(nullptr)
{
	std::string pool = TreeNode.GetPszProperty("pool", "", false);
	if (pool != "")
	{
		m_pool = CEngine::GetSingleton().getRenderableObjectTechniqueManager()->getPool(pool);
	}
}

CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{

}


void CGenerateShadowMapsSceneRendererCommand::Execute(CContextManager &_context)
{
	/*El método Execute recorrerá las luces del lightmanager y comprobará si la luz ha
	de generar ShadowMap y si está activa, en ese caso llamará al método
	SetShadowMap de la luz que establecerá las matrices de View y Projección,
	realizará el clear del zbuffer y del stencil buffer y pintará las capas que el
	shadowmap tenga aplicadas.
	*/
	SCOPED_DEBUG_PERF(L"Generate Shadowmaps");
	if (m_pool)
	{
		m_pool->Apply();
	}
	CEngine::GetSingleton().getLightManager()
		->ExecuteShadowCreation(_context);
}
