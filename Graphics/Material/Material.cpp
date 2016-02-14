#include "Material.h"
#include "Effect/EffectTechnique.h"
#include "Engine/Engine.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode)
	: CNamed(TreeNode)
{
	m_effectTechnique = CEngine::GetSingletonPtr()->getEffectsManager()->get(TreeNode.GetPszProperty("effect_technique"));

	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Texture = TreeNode(i);
		CTexture *texture = CEngine::GetSingleton().getTextureManager()->GetTexture(l_Texture.GetPszProperty("filename"));
		m_textures.push_back(texture);
	}
}


CMaterial::~CMaterial()
{

}


void CMaterial::apply()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		m_textures[i]->Activate(i);
	}
}


CEffectTechnique * CMaterial::getEffectTechnique() const
{
	return m_effectTechnique;
}

void CMaterial::destroy()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		delete m_textures[i];
	}

	m_textures.clear();
}