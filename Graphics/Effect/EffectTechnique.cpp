#include "EffectTechnique.h"
#include "Engine/Engine.h"


CEffectTechnique::CEffectTechnique(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	m_PixelShaderName = TreeNode.GetPszProperty("pixel_shader");
	m_VertexShaderName = TreeNode.GetPszProperty("vertex_shader");
	m_PixelShader = CEngine::GetSingletonPtr()->getEffectsManager()->GetPixelShader(m_PixelShaderName);
	m_VertexShader = CEngine::GetSingletonPtr()->getEffectsManager()->GetVertexShader(m_VertexShaderName);
}


CEffectTechnique::~CEffectTechnique()
{
}


CEffectVertexShader * CEffectTechnique::GetVertexShader()
{
	return m_VertexShader;
}


CEffectPixelShader * CEffectTechnique::GetPixelShader()
{
	return m_PixelShader;
}


void CEffectTechnique::Refresh()
{
	m_VertexShader = CEngine::GetSingleton().getEffectsManager()->GetVertexShader(m_VertexShaderName);
	m_PixelShader = CEngine::GetSingleton().getEffectsManager()->GetPixelShader(m_PixelShaderName);
}


void CEffectTechnique::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	m_VertexShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
	m_PixelShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
}

void CEffectTechnique::destroy()
{
	m_VertexShader->destroy();
	m_PixelShader->destroy();

	m_VertexShader = NULL;
	m_PixelShader = NULL;
}