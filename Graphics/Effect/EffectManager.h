#pragma once

#include <Math/Matrix44.h>
#include <Utils/TMapManager.h>

#define MAXBONES 29

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


class CEffectTechnique;
class CEffectVertexShader;
class CEffectPixelShader;

class CEffectManager : public TMapManager<CEffectTechnique>
{
private:
	TMapManager<CEffectVertexShader> m_VertexShaders;
	TMapManager<CEffectPixelShader> m_PixelShaders;

public:
	static CEffectParameters m_Parameters;

	CEffectManager();
	virtual ~CEffectManager();
	void Reload();
	void load( const std::string &Filename);
	CEffectVertexShader * GetVertexShader( const std::string &VertexShader);
	CEffectPixelShader * GetPixelShader( const std::string &PixelShader);

	void destroy();
};
