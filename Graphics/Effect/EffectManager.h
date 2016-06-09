#pragma once

#include <Base/Math/Matrix44.h>
#include <Base/Utils/TMapManager.h>
#include "EffectParameters.h"

class CEffectTechnique;
class CEffectVertexShader;
class CEffectPixelShader;
class CEffectGeometryShader;
class CLight;

class CEffectManager : public TMapManager<CEffectTechnique>
{
private:
	TMapManager<CEffectVertexShader> m_VertexShaders;
	TMapManager<CEffectPixelShader> m_PixelShaders;
	TMapManager<CEffectGeometryShader> m_GeometryShaders;
	std::string m_FileName;

public:
	static CSceneEffectParameters m_SceneParameters;
	static CAnimatedModelEffectParameters m_AnimatedModelEffectParameters;
	static CLightEffectParameters m_LightParameters;
	static CMaterialEffectParameters m_MaterialEffectParameters;

	CEffectManager();
	virtual ~CEffectManager();
	void Reload();
	void load( const std::string &Filename);
	CEffectVertexShader * GetVertexShader( const std::string &VertexShader);
	CEffectPixelShader * GetPixelShader( const std::string &PixelShader);
	CEffectGeometryShader * GetGeometryShader( const std::string &GeometryShader);

	TMapManager<CEffectVertexShader>::Ref CEffectManager::GetVertexShaderRef(const std::string &VertexShader);
	TMapManager<CEffectPixelShader>::Ref CEffectManager::GetPixelShaderRef(const std::string &PixelShader);
	TMapManager<CEffectGeometryShader>::Ref CEffectManager::GetGeometryShaderRef(const std::string &GeometryShader);

	void SetSceneConstants();
	void SetLightConstants(unsigned int IdLight, CLight *Light);
	void SetLightsConstants();
	void SetMaterialsConstants();
	void SetSceneConstants( CEffectTechnique* technique );
	void SetMaterialConstants( CEffectTechnique* technique );

	void destroy();
};
