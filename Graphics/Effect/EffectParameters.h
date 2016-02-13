#ifndef EFFECT_PARAMENTERS_H
#define EFFECT_PARAMENTERS_H

#include "Math\Matrix44.h"

#define MAXBONES 40
#define MAX_LIGHTS_BY_SHADER 4

class CEffectParameters
{
public:
	Mat44f    m_World;
	Mat44f    m_View;
	Mat44f    m_Projection;
	Vect4f    m_BaseColor;
	Mat44f    m_Bones[MAXBONES];
	Vect4f    m_CameraRightVector;
	Vect4f    m_CameraUpVector;
	Vect4f    m_BillboardPosition;
	Vect4f    m_BillboardSize;
	Vect4f    m_DebugRenderScale;
};

class CSceneEffectParameters
{
public:
	Mat44f m_World;
	Mat44f m_View;
	Mat44f m_Projection;
	Mat44f m_ViewInverse;
	Mat44f m_ProjectionInverse;
	Vect4f m_CameraPosition;
	Vect4f m_CameraRightVector;
	Vect4f m_CameraUpVector;
	Vect4f m_BaseColor;
	Vect4f m_LightAmbient;
};

class CAnimatedModelEffectParameters
{
public:
	Mat44f m_Bones[MAXBONES];
};

class CLightEffectParameters
{
public:
	float m_LightEnabled[MAX_LIGHTS_BY_SHADER];
	float m_LightType[MAX_LIGHTS_BY_SHADER];
	Vect4f m_LightPosition[MAX_LIGHTS_BY_SHADER];
	Vect4f m_LightDirection[MAX_LIGHTS_BY_SHADER];
	float m_LightAngle[MAX_LIGHTS_BY_SHADER];
	float m_LightFallOffAngle[MAX_LIGHTS_BY_SHADER];
	float m_LightAttenuationStartRange[MAX_LIGHTS_BY_SHADER];
	float m_LightAttenuationEndRange[MAX_LIGHTS_BY_SHADER];
	float m_LightIntensity[MAX_LIGHTS_BY_SHADER];
	Vect4f m_LightColor[MAX_LIGHTS_BY_SHADER];
};

#endif