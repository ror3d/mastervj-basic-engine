#include "ApplyFiltersSceneRendererCommand.h"
#include <Engine/Engine.h>
#include "Renderable/RenderableObjectTechnique.h"

#include <Core/Debug/DebugPerf.h>

CApplyFiltersSceneRendererCommand::CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode texture = TreeNode(i);

		if (texture.GetName() == std::string("texture"))
		{
			AddStageTexture(texture.GetIntProperty("stage_id"), CEngine::GetSingleton().getTextureManager()->get(texture.GetPszProperty("file")));
		}
		else if (texture.GetName() == std::string("dynamic_texture"))
		{
			CDynamicTexture * dynText = new CDynamicTexture(texture);
			CEngine::GetSingleton().getTextureManager()->add(dynText->getName(), dynText);
			m_DynamicTextures.push_back(CEngine::GetSingleton().getTextureManager()->get(dynText->getName()));

			m_Materials.push_back(CEngine::GetSingleton().getMaterialManager()->get(texture.GetPszProperty("material")));
		}
	}
}

CApplyFiltersSceneRendererCommand::~CApplyFiltersSceneRendererCommand()
{
	m_Materials.clear();
}

void CApplyFiltersSceneRendererCommand::Execute(CContextManager &_context)
{
	SCOPED_DEBUG_PERF(L"Apply Filter");

	ActivateTextures();

	for (int i = 0; i < m_DynamicTextures.size(); ++i)
	{
		auto text = dynamic_cast<CDynamicTexture *>(m_DynamicTextures[i].getRef());

		ID3D11RenderTargetView *l_RenderTargetViews[1];

		l_RenderTargetViews[0] = text->GetRenderTargetView();

		_context.SetRenderTargets(1, l_RenderTargetViews, text->GetDepthStencilView());

		m_Materials[i]->apply();

		_context.DrawScreenQuad(m_Materials[i]->getRenderableObjectTechique()->GetEffectTechnique(),
			nullptr, 0, 0, text->GetWidth(), text->GetHeight(), CColor(1, 1, 1, 1));

		_context.UnsetRenderTargets();

		m_DynamicTextures[i]->Activate(0);
	}
}