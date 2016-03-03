#include "Scene/DrawQuadSceneRendererCommand.h"
#include "Material/Material.h"
#include "Renderable/RenderableObjectTechnique.h"
#include "Engine/Engine.h"
#include "VertexTypes.h"

CDrawQuadSceneRendererCommand::CDrawQuadSceneRendererCommand(CXMLTreeNode &TreeNode)
	: CStagedTexturedSceneRendererCommand(TreeNode)
{
	/*<render_draw_quad material="DeferredFogMaterial">
		<texture stage_id="0" file="DeferredCapturedTexture"/>
		<texture stage_id="1" file="DepthMapTexture"/>
	</render_draw_quad>*/
	auto mm = CEngine::GetSingleton().getMaterialManager();
	m_Material = mm->get(TreeNode.GetPszProperty("material"));

	for (int i = 0; i < TreeNode.GetNumChildren(); i++)
	{
		CXMLTreeNode texChild = TreeNode(i);
		if ( texChild.GetName() == std::string( "texture" ) )
		{
			CTexture *tex = nullptr;
			if ( texChild.GetBoolProperty( "load_file", false ) )
			{
				tex = new CTexture();
				tex->load( texChild.GetPszProperty( "file" ) );
				CEngine::GetSingleton().getTextureManager()->add( tex->getName(), tex );
			}
			else
			{
				tex = CEngine::GetSingleton().getTextureManager()->GetTexture( texChild.GetPszProperty( "file" ) );
			}
			AddStageTexture( texChild.GetIntProperty( "stage_id" ), tex );
		}
	}
}

void CDrawQuadSceneRendererCommand::Execute(CContextManager &_context)
{
	ActivateTextures();
	m_Material->apply();
	_context.DrawRelativeScreenQuad(m_Material->getRenderableObjectTechique()->GetEffectTechnique(),
							nullptr, 0, 0, 1, 1, CColor(1, 1, 1, 1));
}