#include "EffectManager.h"
#include "EffectTechnique.h"
#include "Effect/EffectVertexShader.h"
#include "Effect/EffectPixelShader.h"

#include <Core/Engine/Engine.h>
#include "Light/Light.h"
#include "Light/LightManager.h"

#include <XML/XMLTreeNode.h>

CSceneEffectParameters CEffectManager::m_SceneParameters;
CAnimatedModelEffectParameters CEffectManager::m_AnimatedModelEffectParameters;
CLightEffectParameters CEffectManager::m_LightParameters;

CEffectManager::CEffectManager()
{
}


CEffectManager::~CEffectManager()
{
}


void CEffectManager::Reload()
{
}

void CEffectManager::destroy()
{
	TMapManager::destroy();
	m_VertexShaders.destroy();
	m_PixelShaders.destroy();
}

void CEffectManager::load(const std::string &Filename)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Effects = l_XML["effects"];
		if (l_Effects.Exists())
		{
			for (int i = 0; i < l_Effects.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Effect = l_Effects(i);

				if (l_Effect.GetName() == std::string("effect_technique"))
				{
					CEffectTechnique * Effect = new CEffectTechnique(l_Effect);
					add(Effect->getName(), Effect);
				}
				else if (l_Effect.GetName() == std::string("vertex_shader"))
				{
					CEffectVertexShader * Effect = new CEffectVertexShader(l_Effect);
					m_VertexShaders.add(Effect->getName(), Effect);
				}
				else if (l_Effect.GetName() == std::string("pixel_shader"))
				{
					CEffectPixelShader * Effect = new CEffectPixelShader(l_Effect);
					m_PixelShaders.add(Effect->getName(), Effect);
				}
			}
		}
	}
}


CEffectVertexShader * CEffectManager::GetVertexShader(const std::string &VertexShader)
{
	return m_VertexShaders.get(VertexShader);
}


CEffectPixelShader * CEffectManager::GetPixelShader(const std::string &PixelShader)
{
	return m_PixelShaders.get(PixelShader);
}

void CEffectManager::SetSceneConstants()
{
	for (auto it : m_resources)
	{
		it.second->SetConstantBuffer(0, &m_SceneParameters);
	}
}

/*void CEffectManager::SetLightConstants(unsigned int IdLight, CLight *Light)
{
	m_LightParameters.m_LightAmbient = (0.1f, 0.1f, 0.1f, 0.0f);
	m_LightParameters.m_LightEnabled[IdLight] = true;
	m_LightParameters.m_LightType[IdLight] = static_cast<float>(Light->getType());
	m_LightParameters.m_LightPosition[IdLight] = Light->getPosition();
	m_LightParameters.m_LightAttenuationStartRange[IdLight] = Light->getStartRangeAttenuation();
	m_LightParameters.m_LightAttenuationEndRange[IdLight] = Light->getEndRangeAttenuation();
	//m_LightParameters.m_LightIntensity[IdLight] = Light->getIntensity();
	m_LightParameters.m_LightColor[IdLight] = Light->getColor();

	if (Light->getType() == CLight::TLightType::DIRECTIONAL)
	{
		m_LightParameters.m_LightDirection[IdLight] = ((CDirectionalLight*)Light)->getDirection();
	}
	else if (Light->getType() == CLight::TLightType::SPOT)
	{
		CSpotLight* l_light = dynamic_cast<CSpotLight*>(Light);
		m_LightParameters.m_LightDirection[IdLight] = l_light->getDirection();
		m_LightParameters.m_LightAngle[IdLight] = l_light->getAngle();
		m_LightParameters.m_LightFallOffAngle[IdLight] = l_light->getFallOff();
	}

}*/

void CEffectManager::SetLightsConstants()
{
	/*CLightManager *l_LightManager = CEngine::GetSingleton().getLightManager();

	size_t i = 0;

	for (size_t i = 0; i < l_LightManager->count(); ++i)
	{
		CLight& l_Light = l_LightManager->iterate(i);
		SetLightConstants(i, &l_Light);
	}

	for (auto it : m_resources)
	{
		it.second->SetConstantBuffer(1, &m_LightParameters);
	}*/
}