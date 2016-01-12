#include "EffectManager.h"
#include "EffectTechnique.h"
#include "Effect/EffectVertexShader.h"
#include "Effect/EffectPixelShader.h"

#include <Core/Engine/Engine.h>
#include "Light/Light.h"
#include "Light/LightManager.h"

#include <XML/XMLTreeNode.h>

CEffectParameters CEffectManager::m_Parameters;
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

}

void CEffectManager::SetLightConstants(unsigned int IdLight, CLight *Light)
{
		m_LightParameters.m_LightEnabled[IdLight] = true;
		m_LightParameters.m_LightAmbient = (1.0f, 1.0f, 1.0f, 1.0f);
		m_LightParameters.m_LightAttenuationEndRange[IdLight] = Light->getEndRangeAttenuation();
		m_LightParameters.m_LightAttenuationEndRange[IdLight] = Light->getStartRangeAttenuation();
		m_LightParameters.m_LightColor[IdLight] = Light->getColor();
		m_LightParameters.m_LightIntensity[IdLight] = Light->getIntensity();

		if (Light->getType() == CLight::DIRECTIONAL)
		{
			m_LightParameters.m_LightDirection[IdLight] = ((CDirectionalLight*)Light)->getDirection();
		}
		else if (Light->getType() == CLight::SPOT)
		{
			m_LightParameters.m_LightDirection[IdLight] = ((CSpotLight*)Light)->getDirection();
			m_LightParameters.m_LightAngle[IdLight] = ((CSpotLight*)Light)->getAngle();
			m_LightParameters.m_LightFallOffAngle[IdLight] = ((CSpotLight*)Light)->getFallOff();
		}
	
}

void CEffectManager::SetLightsConstants(unsigned int MaxLights)
{
	CLightManager *l_LightManager = CEngine::GetSingleton().getLightManager();

	/*for (size_t i = 0; i<MaxLights; ++i)
	{
		m_LightParameters.m_LightAmbient = v4fZERO;
		m_LightParameters.m_LightAngle[i] = 0;
		m_LightParameters.m_LightAttenuationEndRange[i] = 0;
		m_LightParameters.m_LightAttenuationStartRange[i] = 0.0f;
		m_LightParameters.m_LightColor[i] = v4fZERO;
		m_LightParameters.m_LightDirection[i] = v3fZERO;
		m_LightParameters.m_LightEnabled[i] = false;
		m_LightParameters.m_LightFallOffAngle[i] = 0.0f;
		m_LightParameters.m_LightIntensity[i] = 0.0f;
		m_LightParameters.m_LightPosition[i] = v4fZERO;
		m_LightParameters.m_LightType[i] = 0.0f;
	}*/

	size_t i = 0;
	
	for (size_t i = 0; i < MaxLights; ++i)
	{
		CLight l_Light = l_LightManager->iterate(i);
		SetLightConstants(i, &l_Light);
	}

	std::map<std::string, CEffectTechnique*>::iterator itMap;
	for (itMap = m_resources.begin(); itMap != m_resources.end(); ++itMap)
	{
		itMap->second->GetPixelShader()->SetConstantBuffer(1, &m_LightParameters);
		itMap->second->GetVertexShader()->SetConstantBuffer(1, &m_LightParameters);
	}
}
