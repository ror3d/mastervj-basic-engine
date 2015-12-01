#include "Material.h"
#include "Effect/EffectTechnique.h"
#include "Engine/Engine.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode)
	: CNamed(TreeNode)
{
	std::string l_EffectTechnique = TreeNode.GetPszProperty("effect_technique");
	m_effectTechnique = CEngine::GetSingletonPtr()->getEffectsManager()->get(l_EffectTechnique);

	if (l_EffectTechnique == "diffuse_technique")
	{
		for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
		{
			CXMLTreeNode l_Texture = TreeNode(i);
			CTexture * Texture = new CTexture();
			Texture->load(l_Texture.GetPszProperty("filename"));
			m_textures.push_back(Texture);
		}
	}
}


CMaterial::~CMaterial()
{
	destroy();
}


void CMaterial::apply()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		m_textures[i]->Activate(1);
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