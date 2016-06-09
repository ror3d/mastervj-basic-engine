#ifndef LIGHT_H
#define LIGHT_H

#include "Utils/Named.h"
#include "Utils/Active.h"
#include "Utils/Utils.h"
#include "Math/Color.h"
#include "Math/Matrix44.h"
#include <vector>
#include <Base/Utils/TMapManager.h>

class CRenderManager;
class CDynamicTexture;
class CTexture;
class CRenderableObjectsManager;
class CContextManager;

class CLight : public CNamed, public CActive
{
public:
	enum class TLightType
	{
		OMNI = 0,
		DIRECTIONAL,
		SPOT
	};
protected:
	Vect3f m_Position;
	CColor m_Color;
	float m_Intensity;
	float m_StartRangeAttenuation;
	float m_EndRangeAttenuation;

	//ShadowMap
	bool m_GenerateShadowMap;
	CDynamicTexture *m_ShadowMap;
	TMapManager<CTexture>::Ref m_ShadowMaskTexture;
	std::vector<CRenderableObjectsManager *> m_Layers;
	Mat44f m_ViewShadowMap;
	Mat44f m_ProjectionShadowMap;

	Vect2i m_ShadowMapSize;
	std::string m_ShadowMaskFileName;

public:
	CLight(const CXMLTreeNode &TreeNode);
	CLight();
	virtual ~CLight();
	Vect3f getPosition() { return m_Position; } const
	CColor getColor() { return m_Color; } const
	float getIntensity() { return m_Intensity; } const
	float getStartRangeAttenuation() { return m_StartRangeAttenuation; } const
	float getEndRangeAttenuation() { return m_EndRangeAttenuation; } const
	virtual const TLightType getType() const = 0;
	void setPosition(const Vect3f position) { m_Position = position; }
	void setColor(const CColor color) { m_Color = color; }
	void setStartRangeAttenuation(const float startRangeAttenuation) { m_StartRangeAttenuation = startRangeAttenuation; }
	void setEndTangeAttenuation(const float endRangeAttenuation) { m_EndRangeAttenuation = endRangeAttenuation; }
	virtual void Render(CRenderManager *RenderManager);
	static TLightType getLightTypeByName(const std::string &type);

	//ShadowMap
	bool getGenerateShadowMap(){ return m_GenerateShadowMap;  }
	void setGenerateShadowMap(bool generate);
	CDynamicTexture * getShadowMap(){ return m_ShadowMap; }
	CTexture * getShadowMaskTexture(){ return m_ShadowMaskTexture.getRef(); }
	Mat44f getViewShadowMap(){ return m_ViewShadowMap; }
	Mat44f getProjectionShadowMap(){ return m_ProjectionShadowMap; }
	std::vector<CRenderableObjectsManager *> getLayers(){ return m_Layers; }
	virtual void SetShadowMap(CContextManager &_context) = 0;
};

//-----------OMNI
class COmniLight : public CLight
{
public:
	COmniLight();
	COmniLight(const CXMLTreeNode &TreeNode);
	virtual const TLightType getType() const { return TLightType::OMNI; }
	void SetShadowMap(CContextManager &_context);
};

//-----------DIRECTIONAL
class CDirectionalLight : public CLight
{
protected:
	Vect3f m_Direction;
	Vect2f m_OrthoShadowMapSize;
public:
	CDirectionalLight();
	CDirectionalLight(const CXMLTreeNode &TreeNode);
	Vect3f getDirection() { return m_Direction; } const
	void setDiretion(const Vect3f direction) { m_Direction = direction; }
	virtual void Render(CRenderManager *RenderManager);
	virtual const TLightType getType() const { return TLightType::DIRECTIONAL; }
	void SetShadowMap(CContextManager &_context);
};

//-----------SPOT
class CSpotLight : public CDirectionalLight
{
protected:
	float m_Angle;
	float m_FallOff;
public:
	CSpotLight();
	CSpotLight(const CXMLTreeNode &TreeNode);
	float getAngle() { return m_Angle; } const
	float getFallOff() { return m_FallOff; } const
	void setAngle(const float angle) { m_Angle = angle; }
	void setFallOff(const float fallOff) { m_FallOff = fallOff; }
	virtual const TLightType getType() const { return TLightType::SPOT; }
	void SetShadowMap(CContextManager &_context);
};

#endif