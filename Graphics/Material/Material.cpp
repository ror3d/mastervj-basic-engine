#include "Material.h"
#include "Engine/Engine.h"
#include "Graphics/Renderable/RenderableObjectTechnique.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode)
	: CNamed(TreeNode)
{
	/*m_RenderableObjectTechnique = new CRenderableObjectTechnique(TreeNode.GetPszProperty("effect_technique"), 
		CEngine::GetSingletonPtr()->getEffectsManager()->get(TreeNode.GetPszProperty("effect_technique")));*/

	std::string name;
	const char * rot = TreeNode.GetPszProperty("renderable_object_technique", 0, false);
	if (rot)
	{
		name = rot;
	}
	else
	{
		name = TreeNode.GetPszProperty("vertex_type", "", true);
	}

	m_RenderableObjectTechnique = CEngine::GetSingleton().getRenderableObjectTechniqueManager()->get(name);
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Texture = TreeNode(i);
		CTexture *texture = CEngine::GetSingleton().getTextureManager()->GetTexture(l_Texture.GetPszProperty("filename"));
		m_textures.push_back(texture);
	}
}


CMaterial::~CMaterial()
{
	destroy();
}

void CMaterial::apply(CRenderableObjectTechnique *RenderableObjectTechnique)
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		m_textures[i]->Activate(i);
	}
}

void CMaterial::destroy()
{
	m_textures.clear();
}