#include "Renderable\RenderableObjectTechniqueManager.h"

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
{

}
CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
	Destroy();
}
bool CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(CPoolRenderableObjectTechnique	*PoolRenderableObjectTechniques,
	const std::string &RenderableObjectTechniqueName,
	const std::string &TechniqueName)
{	
	CRenderableObjectTechnique * tech = new CRenderableObjectTechnique(RenderableObjectTechniqueName, CEngine::GetSingleton().getEffectsManager()->get(TechniqueName));
	add(RenderableObjectTechniqueName,tech);
	return true;
}

void CRenderableObjectTechniqueManager::Destroy()
{
	m_PoolRenderableObjectTechniques.destroy();
	m_resources.clear();
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
				CXMLTreeNode l_pool = l_renderable_ob_technqs(i);

				if (l_pool.GetName() == std::string("pool_renderable_object_technique")){
					CPoolRenderableObjectTechnique * pool;
					pool->setName(l_pool.GetPszProperty("name"));
					for (int j = 0; j < l_pool.GetNumChildren(); j++){
						CXMLTreeNode l_technique = l_pool(i);
						
						if (l_technique.GetName() == std::string("default_technique")){
							//TODO:
							//l_technique.GetPszProperty("vertex_type");
							//l_technique.GetPszProperty("technique")
						}
						else if (l_technique.GetName() == std::string("renderable_object_technique")){
							InsertRenderableObjectTechnique(pool, l_technique.GetPszProperty("name"), l_technique.GetPszProperty("technique"));
							pool->AddElement(pool->getName(), l_technique.GetPszProperty("technique"), get(l_technique.GetPszProperty("name")));
						}
					}
					m_PoolRenderableObjectTechniques.add(pool->getName(),&pool);
				}
			}
		}
	}
	return true;
}
bool CRenderableObjectTechniqueManager::Reload()
{
	return Load(m_Filename);
}
TMapManager<CPoolRenderableObjectTechnique*> & CRenderableObjectTechniqueManager::GetPoolRenderableObjectTechniques()
{
	return m_PoolRenderableObjectTechniques;
}