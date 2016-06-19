#include "Renderer.h"
#include "Renderable/Renderable.h"
#include "Effect/EffectManager.h"


CRenderer::CRenderer()
{}


CRenderer::~CRenderer()
{}

void CRenderer::AddRenderableToRender(const std::string& layer, IRenderable* mesh, const Mat44f& transf)
{
	m_Layers[layer][mesh].push_back(transf);
}

void CRenderer::RenderLayer(const std::string& layer, CContextManager *context)
{
	auto layerIt = m_Layers.find(layer);

	if (layerIt == m_Layers.end())
	{
		return;
	}

	for (auto &const meshInstances : layerIt->second)
	{
		for (auto &const transf : meshInstances.second)
		{
			CEffectManager::m_SceneParameters.m_World = transf;
			meshInstances.first->Render(context);
		}
	}
}

void CRenderer::BeginRender()
{
}

void CRenderer::EndRender()
{
	m_Layers.clear();
}
