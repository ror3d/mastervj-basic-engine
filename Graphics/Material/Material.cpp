#include "Material.h"
#include "Engine\Engine.h"
#include "Renderable\RenderableObjectTechnique.h"
#include "MaterialParameter.h"

CMaterial::CMaterial(CXMLTreeNode &TreeNode)
	: CNamed(TreeNode)
{
	std::string technique = TreeNode.GetPszProperty("effect_technique");
	m_RenderableObjectTechnique = new CRenderableObjectTechnique(TreeNode.GetPszProperty("effect_technique"), 
		CEngine::GetSingletonPtr()->getEffectsManager()->get(TreeNode.GetPszProperty("effect_technique")));
	void *nextDir = &CEffectManager::m_MaterialEffectParameters.m_RawData[0];
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_paramMat = TreeNode(i);
		if (l_paramMat.GetName() == std::string("texture")){
			CXMLTreeNode l_Texture = l_paramMat;
			CTexture * Texture = new CTexture();
			Texture->load(l_Texture.GetPszProperty("filename"));
			m_textures.push_back(Texture);
		}
		else if (l_paramMat.GetName() == std::string("parameter"))
		{
			CMaterialParameter::TMaterialType type;
			if (l_paramMat.GetPszProperty("type") == std::string("float"))
			{
				type = CMaterialParameter::TMaterialType::FLOAT;
				CTemplatedMaterialParameter<float> *param = new CTemplatedMaterialParameter<float>(
					l_paramMat,
					nextDir,
					l_paramMat.GetFloatProperty("value"),
					type);
				m_Parameters.push_back(param);
			}
			else if (l_paramMat.GetPszProperty("type") == std::string("Vect2f"))
			{
				type = CMaterialParameter::TMaterialType::VECT2F;
				CTemplatedMaterialParameter<Vect2f> *param = new CTemplatedMaterialParameter<Vect2f>(
					l_paramMat,
					nextDir,
					l_paramMat.GetFloatProperty("value"),
					type);
				m_Parameters.push_back(param);
			}
			else if (l_paramMat.GetPszProperty("type") == std::string("Vect3f"))
			{
				type = CMaterialParameter::TMaterialType::VECT3F;
				CTemplatedMaterialParameter<Vect3f> *param = new CTemplatedMaterialParameter<Vect3f>(
					l_paramMat,
					nextDir,
					l_paramMat.GetFloatProperty("value"),
					type);
				m_Parameters.push_back(param);
			}
			else if (l_paramMat.GetPszProperty("type") == std::string("Vect4f"))
			{
				type = CMaterialParameter::TMaterialType::VECT4F;
				CTemplatedMaterialParameter<Vect4f> *param = new CTemplatedMaterialParameter<Vect4f>(
					l_paramMat,
					nextDir,
					l_paramMat.GetFloatProperty("value"),
					type);
				m_Parameters.push_back(param);
			}

			nextDir = reinterpret_cast<unsigned char*>(nextDir)+sizeof(Vect4f);
			
		}
	}
}


CMaterial::~CMaterial()
{
	destroy();
}

void CMaterial::apply(CRenderableObjectTechnique *RenderableObjectTechnique)
{
	for (int i = 0; i < m_Parameters.size(); i++)
	{		
		m_Parameters[i]->Apply();
	}

	CEngine::GetSingleton().getEffectsManager()->SetMaterialsConstants();

	for (int i = 0; i < m_textures.size(); ++i)
	{
		m_textures[i]->Activate(i);
	}

}

void CMaterial::destroy()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		delete m_textures[i];
	}

	m_textures.clear();
}
