#ifndef LIGHT_H
#define LIGHT_H

#include "Utils/Named.h"
#include "Utils\Utils.h"
#include "Math/Color.h"

/*
#include "Graphics\Renderer\3DElement.h"
#include "Graphics\Texture\DynamicTexture.h"
#include <vector>
#include "Graphics\Renderable\RenderableObjectsManager.h"
#include "Engine\Engine.h"*/

class CRenderManager;

class CLight : public CNamed
{
public:
	enum TLightType
	{
		OMNI = 0,
		DIRECTIONAL,
		SPOT
	};
protected:
	Vect3f m_Position;
	CColor m_Color;
	TLightType m_Type;
	float m_Intensity;
	float m_StartRangeAttenuation;
	float m_EndRangeAttenuation;
	/*bool m_GenerateShadowMap;
	CDynamicTexture *m_ShadowMap;
	CTexture *m_ShadowMaskTexture;
	std::vector<CRenderableObjectsManager *> m_Layers;
	Mat44f m_ViewShadowMap, m_ProjectionShadowMap;*/
public:
	CLight(const CXMLTreeNode &TreeNode);
	CLight();
	virtual ~CLight();
	Vect3f getPosition() { return m_Position; } const
	CColor getColor() { return m_Color; } const
	float getStartRangeAttenuation() { return m_StartRangeAttenuation; } const
	float getEndRangeAttenuation() { return m_EndRangeAttenuation; } const
	TLightType getType() { return m_Type; } const
	void setPosition(const Vect3f position) { m_Position = position; }
	void setColor(const CColor color) { m_Color = color; }
	void setStartRangeAttenuation(const float startRangeAttenuation) { m_StartRangeAttenuation = startRangeAttenuation; }
	void setEndTangeAttenuation(const float endRangeAttenuation) { m_EndRangeAttenuation = endRangeAttenuation; }
	void setType(const TLightType type) { m_Type = type; }
	virtual void Render(CRenderManager *RenderManager);
	static TLightType getLightTypeByName(const std::string &type);
	/*TODO:
	UAB_GET_SET_GET_ADRESS_STANDARD_PROPERTY(bool, GenerateShadowMap);
	UAB_GET_PROPERTY_POINTER(CDynamicTexture, ShadowMap);
	UAB_GET_PROPERTY_POINTER(CTexture, ShadowMaskTexture);
	UAB_GET_PROPERTY_REFERENCE(Mat44f, ViewShadowMap);
	UAB_GET_PROPERTY_REFERENCE(Mat44f, ProjectionShadowMap);
	UAB_GET_PROPERTY_REFERENCE(std::vector<CRenderableObjectsManager *>, Layers);*/
	//virtual void SetShadowMap(CRenderManager &RenderManager);
};


class COmniLight : public CLight
{
public:
	COmniLight();
	COmniLight(const CXMLTreeNode &TreeNode);
};


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
	void SetShadowMap(CRenderManager &RenderManager);
};


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
};

#endif