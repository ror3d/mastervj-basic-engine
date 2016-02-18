#include "Renderable\RenderableObjectTechniqueManager.h"

#include "Renderable\RenderableObjectTechnique.h"
#include "Renderable\PoolRenderableObjectTechnique.h"

#include "Engine\Engine.h"

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
	: m_Filename("")
{

}

CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
	destroy();
}

bool CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(CPoolRenderableObjectTechnique	*PoolRenderableObjectTechniques,
	const std::string &RenderableObjectTechniqueName,
	const std::string &TechniqueName)
{
	CRenderableObjectTechnique *l_RenderableObjectTechniqueOnRRenderableObjectTechniqueManager = get(RenderableObjectTechniqueName);
	
	if (l_RenderableObjectTechniqueOnRRenderableObjectTechniqueManager == NULL)
	{
		CEffectTechnique *l_EffectTechnique = CEngine::GetSingleton().getEffectsManager()->get(TechniqueName);
		l_RenderableObjectTechniqueOnRRenderableObjectTechniqueManager = new CRenderableObjectTechnique(RenderableObjectTechniqueName, l_EffectTechnique);
		add(l_RenderableObjectTechniqueOnRRenderableObjectTechniqueManager->getName(), l_RenderableObjectTechniqueOnRRenderableObjectTechniqueManager);
	}
	PoolRenderableObjectTechniques->AddElement(RenderableObjectTechniqueName, TechniqueName, l_RenderableObjectTechniqueOnRRenderableObjectTechniqueManager);
	return true;
}

void CRenderableObjectTechniqueManager::destroy()
{
	m_PoolRenderableObjectTechniques.destroy();
	TMapManager<CRenderableObjectTechnique>::destroy();
}

bool CRenderableObjectTechniqueManager::Load(const std::string &FileName)
{
	m_Filename = FileName;
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_renderable_ob_technqs = l_XML["renderable_object_techniques"];
		if (l_renderable_ob_technqs.Exists())
		{
			for (int i = 0; i < l_renderable_ob_technqs.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Pool = l_renderable_ob_technqs(i);

				if (l_Pool.GetName() == std::string("pool_renderable_object_technique"))
				{
					CPoolRenderableObjectTechnique * l_PoolRenderableObjectTechnique = new CPoolRenderableObjectTechnique(l_Pool);
					for (int j = 0; j < l_Pool.GetNumChildren(); j++)
					{
						CXMLTreeNode l_Technique = l_Pool(j);
						std::string l_RenderableObjectTechniqueName;
						std::string l_TechniqueName;
						if (l_Technique.GetName() == std::string("default_technique"))
						{
							l_RenderableObjectTechniqueName = l_Technique.GetPszProperty("vertex_type", "");
							l_TechniqueName = l_Technique.GetPszProperty("technique", "");
						}
						else if (l_Technique.GetName() == std::string("renderable_object_technique"))
						{
							l_RenderableObjectTechniqueName = l_Technique.GetPszProperty("name", "");
							l_TechniqueName = l_Technique.GetPszProperty("technique", "");
						}
						InsertRenderableObjectTechnique(l_PoolRenderableObjectTechnique, l_RenderableObjectTechniqueName, l_TechniqueName);
					}
					m_PoolRenderableObjectTechniques.add(l_PoolRenderableObjectTechnique->getName(), l_PoolRenderableObjectTechnique);
				}
			}
		}
		return true;
	}
	return false;
	
}

bool CRenderableObjectTechniqueManager::Reload()
{
	destroy();
	return Load(m_Filename);
}

TMapManager<CPoolRenderableObjectTechnique> & CRenderableObjectTechniqueManager::GetPoolRenderableObjectTechniques()
{
	return m_PoolRenderableObjectTechniques;
}