#include "Light.h"
#include <Engine/Engine.h>


CLight::CLight() : CNamed("")
{
}


CLight::CLight(const CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	Vect3f pos(0.0f, 0.0f, 0.0f);
	m_Position = TreeNode.GetVect3fProperty("pos", pos);
	Vect4f color(0.0f, 0.0f, 0.0f, 0.0f);
	m_Color = CColor(TreeNode.GetVect4fProperty("color", color));
	m_StartRangeAttenuation = TreeNode.GetFloatProperty("att_start_range");
	m_EndRangeAttenuation = TreeNode.GetFloatProperty("att_end_range");
	m_Intensity = TreeNode.GetFloatProperty("intensity");
}


CLight::TLightType CLight::getLightTypeByName(const std::string &type)
{
	if (type == "omni")
	{
		return CLight::TLightType::OMNI;
	}
	else if (type == "directional")
	{
		return CLight::TLightType::DIRECTIONAL;
	}
	else if (type == "spot")
	{
		return CLight::TLightType::SPOT;
	}
	else
	{
		return (CLight::TLightType) - 1;
	}
}


CLight::~CLight()
{
}


void CLight::Render(CRenderManager *RenderManager)
{
	CEffectManager l_effectManager = *(CEngine::GetSingleton().getEffectsManager());

	l_effectManager.SetLightsConstants();
}


COmniLight::COmniLight(const CXMLTreeNode &TreeNode) : CLight(TreeNode)
{

}


CDirectionalLight::CDirectionalLight(const CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	Vect3f dir(0.0f, 0.0f, 0.0f);
	m_Direction = TreeNode.GetVect3fProperty("dir", dir);
}


void CDirectionalLight::Render(CRenderManager *RenderManager)
{
	CEngine::GetSingleton().getEffectsManager()->SetLightsConstants();
}


CSpotLight::CSpotLight(const CXMLTreeNode &TreeNode) : CDirectionalLight(TreeNode)
{
	m_Angle = TreeNode.GetFloatProperty("angle");
	m_FallOff = TreeNode.GetFloatProperty("falloff");
}