#include "LayerManager.h"
#include "Mesh/MeshInstance.h"
#include "Renderable/AnimatedInstanceModel.h"

CLayerManager::CLayerManager(){

}

CLayerManager::~CLayerManager(){
	Destroy();
}

void CLayerManager::Destroy(){
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->destroy();
	}
}
void CLayerManager::Load(const std::string &FileName){
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_MeshesInfo = l_XML["renderable_objects"];
		if (l_MeshesInfo.Exists())
		{
			for (int i = 0; i < l_MeshesInfo.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_MeshInfo = l_MeshesInfo(i);

				if (l_MeshInfo.GetName() == std::string("layer")){
					CRenderableObjectsManager * renderObManager = new CRenderableObjectsManager();
					add(l_MeshInfo.GetPszProperty("name"), renderObManager);
					if (l_MeshInfo.GetBoolProperty("default", false))
						m_DefaultLayer = renderObManager;
				}
				if (l_MeshInfo.GetName() != std::string("layer") && m_resources.size() == 0){
					CRenderableObjectsManager * renderObManager = new CRenderableObjectsManager();
					add("default", renderObManager);
					m_DefaultLayer = renderObManager;
				}
			    if(l_MeshInfo.GetName() == std::string("mesh_instance"))
				{
					m_DefaultLayer->AddMeshInstance(l_MeshInfo);
				}
				else if (l_MeshInfo.GetName() == std::string("animated_instance"))
				{
					m_DefaultLayer->AddAnimatedInstanceModel(l_MeshInfo);
				}
			}
		}
	}
}

void CLayerManager::Reload(){
	Load(m_Filename);
}

void CLayerManager::Update(float ElapsedTime){
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Update(ElapsedTime);
	}
}
void CLayerManager::Render(CContextManager &_context){
	for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
	{
		it->second->Render(&_context);
	}	
}

void CLayerManager::Render(CContextManager *_context, const std::string &LayerName){
	auto it = m_resources.find(LayerName);

	if (it != m_resources.end())
	{
		it->second->Render(_context);
	}
}