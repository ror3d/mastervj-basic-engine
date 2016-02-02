#include "Material.h"
#include "Engine\Engine.h"
#include "Graphics\Renderable\RenderableObjectTechnique.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode)
	: CNamed(TreeNode)
{
	//m_effectTechnique = CEngine::GetSingletonPtr()->getEffectsManager()->get(TreeNode.GetPszProperty("effect_technique"));
	m_RenderableObjectTechnique = new CRenderableObjectTechnique(TreeNode.GetPszProperty("effect_technique"), 
		CEngine::GetSingletonPtr()->getEffectsManager()->get(TreeNode.GetPszProperty("effect_technique")));
	//m_RenderableObjectTechnique->SetEffectTechnique(CEngine::GetSingletonPtr()->getEffectsManager()->get(TreeNode.GetPszProperty("effect_technique")));

	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Texture = TreeNode(i);
		CTexture * Texture = new CTexture();
		Texture->load(l_Texture.GetPszProperty("filename"));
		m_textures.push_back(Texture);
	}
}


CMaterial::~CMaterial()
{
	destroy();
}


void CMaterial::apply(CRenderableObjectTechnique *RenderableObjectTechnique)
{
	/*for (int i = 0; i < m_textures.size(); ++i)
	{
		m_textures[i]->Activate(i);
	}*/
}

void CMaterial::destroy()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		delete m_textures[i];
	}

	m_textures.clear();
}