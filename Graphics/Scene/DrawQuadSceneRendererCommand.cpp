#include "Scene/DrawQuadSceneRendererCommand.h"

#include "Engine/Engine.h"
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Base/XML/XMLTreeNode.h>
#include "Material/Material.h"
#include "Renderable/RenderableObjectTechnique.h"

CDrawQuadSceneRendererCommand::CDrawQuadSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CStagedTexturedSceneRendererCommand(TreeNode)
{
	/*<render_draw_quad material="DeferredFogMaterial">
		<texture stage_id="0" file="DeferredCapturedTexture"/>
		<texture stage_id="1" file="DepthMapTexture"/>
	</render_draw_quad>*/
	auto mm = CEngine::GetSingleton().getMaterialManager();
	std::string nameMat = TreeNode.GetPszProperty("material");
	m_Material = mm->ref(TreeNode.GetPszProperty("material"));

	for (int i = 0; i < TreeNode.GetNumChildren(); i++)
	{
		CXMLTreeNode texChild = TreeNode(i);
		if (texChild.GetName() == std::string("texture"))
		{
			CTexture *tex = nullptr;
			if (texChild.GetBoolProperty("load_file", false))
			{
				tex = CEngine::GetSingleton().getTextureManager()->GetTexture(texChild.GetPszProperty("file"));
			}
			else
			{
				tex = CEngine::GetSingleton().getTextureManager()->get(texChild.GetPszProperty("file"));
			}

			AddStageTexture(texChild.GetIntProperty("stage_id"), CEngine::GetSingleton().getTextureManager()->ref(tex->getName()));
		}
	}
}

void CDrawQuadSceneRendererCommand::Execute(CContextManager &_context)
{
	ActivateTextures();
	m_Material->apply();
	auto mat = m_Material->getName();
	_context.DrawRelativeScreenQuad(m_Material->getRenderableObjectTechique()->GetEffectTechnique() , nullptr, 0, 0, 1, 1, CColor(1, 1, 1, 1));
}