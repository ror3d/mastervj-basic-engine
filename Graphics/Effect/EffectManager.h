#pragma once

#include <Math/Matrix44.h>
#include <Utils/TMapManager.h>
#include "EffectParameters.h"

class CEffectTechnique;
class CEffectVertexShader;
class CEffectPixelShader;
class CLight;

class CEffectManager : public TMapManager<CEffectTechnique>
{
private:
	TMapManager<CEffectVertexShader> m_VertexShaders;
	TMapManager<CEffectPixelShader> m_PixelShaders;

public:
	static CEffectParameters m_Parameters;
	static CSceneEffectParameters m_SceneParameters;
	static CAnimatedModelEffectParameters m_AnimatedModelEffectParameters;
	static CLightEffectParameters m_LightParameters;


	CEffectManager();
	virtual ~CEffectManager();
	void Reload();
	void load( const std::string &Filename);
	CEffectVertexShader * GetVertexShader( const std::string &VertexShader);
	CEffectPixelShader * GetPixelShader( const std::string &PixelShader);

	void SetSceneConstants();
	void SetLightConstants(unsigned int IdLight, CLight *Light);
	void SetLightsConstants();

	void destroy();
};
