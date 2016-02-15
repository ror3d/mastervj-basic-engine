#include "LightManager.h"
#include <Engine/Engine.h>


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
		m_FileName = FileName;

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

CLight& CLightManager::iterate(size_t id)
{
	int i = 0;
	CLight *l_Light;
	for (auto it : m_resources)
	{
		if (i == id)
		{
			l_Light = it.second;
		}
		i++;
	}

	return *l_Light;
}

size_t CLightManager::count()
{
	size_t i = 0;

	for (auto it : m_resources)
	{
		i++;
	}

	return i;
}

void CLightManager::reload()
{
	destroy();
	Load(m_FileName);
}