#include "LightManager.h"
#include "Light/Light.h"


CLightManager::CLightManager()
{
}


CLightManager::~CLightManager()
{
}


void CLightManager::Load(const std::string &FileName)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_Lights = l_XML["lights"];
		if (l_Lights.Exists())
		{
			for (int i = 0; i < l_Lights.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Light = l_Lights(i);

				CLight::TLightType type = CLight::getLightTypeByName(l_Light.GetPszProperty("type"));

				if (type == CLight::TLightType::OMNI)
				{
					COmniLight * light = new COmniLight(l_Light);
					add(light->getName(), light);
				}
				else if (type == CLight::TLightType::DIRECTIONAL)
				{
					CDirectionalLight * light = new CDirectionalLight(l_Light);
					add(light->getName(), light);
				}
				else if (type == CLight::TLightType::SPOT)
				{
					CSpotLight * light = new CSpotLight(l_Light);
					add(light->getName(), light);
				}				
			}
		}
	}
}


void CLightManager::Render(CRenderManager *RenderManager)
{
	for (auto it : m_resources)
	{
		it.second->Render(RenderManager);
	}
}