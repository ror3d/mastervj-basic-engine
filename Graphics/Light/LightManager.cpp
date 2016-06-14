#include "LightManager.h"
#include "Light/Light.h"
#include <Engine/Engine.h>
#include <Graphics/Context/ContextManager.h>
#include "Renderable/RenderableObjectsManager.h"
#include <Base/XML/XMLTreeNode.h>

#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Camera/CameraController.h"


CLightManager::CLightManager()
	: m_ambient(0, 0, 0, 0)
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
				if ( l_Light.GetName() == std::string("light") )
				{
					std::string typeName = l_Light.GetPszProperty("type");
					if (typeName == "ambient")
					{
						Vect4f c = l_Light.GetVect4fProperty("color", Vect4f(0, 0, 0, 0), false);
						m_ambient = CColor(c.x, c.y, c.z, c.w);
						continue;
					}
					CLight::TLightType type = CLight::getLightTypeByName(typeName);

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

}


void CLightManager::Render(CRenderManager *RenderManager)
{
	for (auto const &it : m_resources)
	{
		it.second->Render(RenderManager);
	}
}

CLight& CLightManager::iterate(size_t id)
{
	int i = 0;
	CLight *l_Light;
	for (auto const &it : m_resources)
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
	for (auto const &it : m_resources)
	{
		i++;
	}
	return i;
}

void CLightManager::ExecuteShadowCreation(CContextManager &_context)
{
	CCamera cam;
	ICameraController *cc = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();

	cc->UpdateCameraValues(&cam);

	for (auto const &light : m_resources)
	{
		/* TODO(roc): reenable
		if (light.second->getGenerateShadowMap() && light.second->isActive())
		{
			light.second->SetShadowMap(_context, cam); //Set matrices y renderTarget

			auto c = _context.m_BackgroundColor;
			_context.m_BackgroundColor = CColor(1, 1, 1, 1);
			_context.Clear(true, true);//Clear Depth
			_context.m_BackgroundColor = c;

			std::vector<CRenderableObjectsManager *> layers = light.second->getLayers();
			for (auto child = layers.begin(); child < layers.end(); child++)
			{
				(*child)->Render(&_context);//Render de layers afectadas por la luz
			}
			//DUDA::::::DONDE SE USA m_ShadowMaskTexture???
		}*/
	}
	_context.UnsetRenderTargets();//Una vez pintadas las sombras, quitamos target para render normal
}

void CLightManager::reload()
{
	destroy();
	Load(m_FileName);
}
