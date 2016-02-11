#include "Light.h"
#include <Engine/Engine.h>
#include "Texture/DynamicTexture.h"
#include "Context/ContextManager.h"


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
	m_isActive = true;
	//m_GenerateShadowMap = TreeNode.GetBoolProperty("generate_shadow_map",false);
	m_GenerateShadowMap = false;

	if (m_GenerateShadowMap){
		m_ShadowMap = new CDynamicTexture(TreeNode.GetPszProperty("shadow_texture_mask"), TreeNode.GetIntProperty("shadow_map_width"), TreeNode.GetIntProperty("shadow_map_height"), true);
		m_ShadowMaskTexture = new CTexture();		
	}

	CXMLTreeNode light = TreeNode;
	for (int i = 0; i< light.GetNumChildren(); i++){
		CXMLTreeNode layer = light(i);
		if (layer.GetName() == std::string("layer")){
			m_Layers.push_back(CEngine::GetSingleton().getLayerManager()->get(light(i).GetPszProperty("layer")));
		}			
	}	
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



//-----------OMNI
COmniLight::COmniLight(const CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
}
void COmniLight::SetShadowMap(CContextManager &_context){
	
}

//-----------DIRECTIONAL
CDirectionalLight::CDirectionalLight(const CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	Vect3f dir(0.0f, 0.0f, 0.0f);
	m_Direction = TreeNode.GetVect3fProperty("dir", dir);
}


void CDirectionalLight::Render(CRenderManager *RenderManager)
{
	CEngine::GetSingleton().getEffectsManager()->SetLightsConstants();
}

void CDirectionalLight::SetShadowMap(CContextManager &_context)
{
	if (m_ShadowMap == NULL)
		assert(false);
	m_ViewShadowMap.SetIdentity();
	m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + m_Direction, v3fY);
	unsigned int l_ShadowMapWidth = m_ShadowMap->GetWidth();
	unsigned int l_ShadowMapHeight = m_ShadowMap->GetHeight();
	m_ProjectionShadowMap.SetFromOrtho(m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 0.1f, m_EndRangeAttenuation);
	CEffectManager::m_SceneParameters.m_View = m_ViewShadowMap;
	CEffectManager::m_SceneParameters.m_Projection = m_ProjectionShadowMap;
	ID3D11RenderTargetView *l_RenderTargetViews[1];
	l_RenderTargetViews[0] = m_ShadowMap->GetRenderTargetView();
	D3D11_VIEWPORT m_viewport;
	m_viewport.Width = (float)l_ShadowMapWidth;
	m_viewport.Height = (float)l_ShadowMapHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	_context.GetDeviceContext()->RSSetViewports(1, &m_viewport);
	_context.SetRenderTargets(1, l_RenderTargetViews, m_ShadowMap->GetDepthStencilView());
}

//-----------SPOT
CSpotLight::CSpotLight(const CXMLTreeNode &TreeNode) : CDirectionalLight(TreeNode)
{
	Vect2f orthoSize(0.0f, 0.0f);
	m_OrthoShadowMapSize = TreeNode.GetVect2fProperty("ortho_shadow_map_size", orthoSize);
	m_Angle = TreeNode.GetFloatProperty("angle");
	m_FallOff = TreeNode.GetFloatProperty("falloff");
}

void CSpotLight::SetShadowMap(CContextManager &_context)
{
	if (m_ShadowMap == NULL)
		assert(false);
	m_ViewShadowMap.SetIdentity();
	m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + m_Direction, v3fY);
	unsigned int l_ShadowMapWidth = m_ShadowMap->GetWidth();
	unsigned int l_ShadowMapHeight = m_ShadowMap->GetHeight();
	m_ProjectionShadowMap.SetIdentity();
	m_ProjectionShadowMap.SetFromPerspective(m_FallOff, l_ShadowMapWidth / (float)l_ShadowMapHeight, 0.1f, m_EndRangeAttenuation);
	CEffectManager::m_SceneParameters.m_View = m_ViewShadowMap;
	CEffectManager::m_SceneParameters.m_Projection = m_ProjectionShadowMap;
	ID3D11RenderTargetView *l_RenderTargetViews[1];
	l_RenderTargetViews[0] = m_ShadowMap->GetRenderTargetView();

	D3D11_VIEWPORT m_viewport;
	m_viewport.Width = (float)l_ShadowMapWidth;
	m_viewport.Height = (float)l_ShadowMapHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	_context.GetDeviceContext()->RSSetViewports(1, &m_viewport);
	_context.SetRenderTargets(1, l_RenderTargetViews, m_ShadowMap->GetDepthStencilView());
}