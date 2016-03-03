#include "Material.h"
#include "Engine/Engine.h"
#include "Graphics/Renderable/RenderableObjectTechnique.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "MaterialParameter.h"

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
		DEBUG_ASSERT( name != std::string("") );
	}

	m_RenderableObjectTechnique = CEngine::GetSingleton().getRenderableObjectTechniqueManager()->get(name);
	void *nextDir = &CEffectManager::m_MaterialEffectParameters.m_RawData[0];
	int l_NextStage = 0;
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_paramMat = TreeNode(i);
		if (l_paramMat.GetName() == std::string("texture"))
		{
			CXMLTreeNode l_Texture = l_paramMat;
			CTexture * Texture = CEngine::GetSingleton().getTextureManager()->GetTexture(l_Texture.GetPszProperty("filename"));
			int stage = GetTextureStage(l_Texture.GetPszProperty("type", "", false));
			if (stage < 0)
			{
				stage = l_paramMat.GetIntProperty("stage", -1, false);
			}
			if (stage < 0)
			{
				stage = l_NextStage;
			}
			m_textures.push_back(std::make_pair(stage,Texture));
			l_NextStage++;
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
					l_paramMat.GetVect2fProperty("value", Vect2f(0, 0)),
					type);
				m_Parameters.push_back(param);
}
			else if (l_paramMat.GetPszProperty("type") == std::string("Vect3f"))
			{
				type = CMaterialParameter::TMaterialType::VECT3F;
				CTemplatedMaterialParameter<Vect3f> *param = new CTemplatedMaterialParameter<Vect3f>(
					l_paramMat,
					nextDir,
					l_paramMat.GetVect3fProperty("value", Vect3f(0, 0, 0)),
					type);
				m_Parameters.push_back(param);
			}
			else if (l_paramMat.GetPszProperty("type") == std::string("Vect4f"))
			{
				type = CMaterialParameter::TMaterialType::VECT4F;
				CTemplatedMaterialParameter<Vect4f> *param = new CTemplatedMaterialParameter<Vect4f>(
					l_paramMat,
					nextDir,
					l_paramMat.GetVect4fProperty("value", Vect4f(0, 0, 0, 0)),
					type);
				m_Parameters.push_back(param);
			}

			nextDir = reinterpret_cast<unsigned char*>(nextDir)+sizeof(Vect4f);
			
		}
	}
}


CMaterial::~CMaterial()
{
	for (auto &param : m_Parameters)
	{
		delete param;
	}
	m_Parameters.clear();
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
		m_textures[i].second->Activate(m_textures[i].first);
	}

}

void CMaterial::destroy()
{
	m_textures.clear();
}