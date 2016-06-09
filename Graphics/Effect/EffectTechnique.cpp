#include "EffectTechnique.h"
#include "EffectPixelShader.h"
#include "EffectVertexShader.h"
#include "EffectGeometryShader.h"

#include <Core/Engine/Engine.h>
#include <Graphics/Effect/EffectManager.h>
#include <Base/XML/XMLTreeNode.h>


CEffectTechnique::CEffectTechnique(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	m_PixelShaderName = TreeNode.GetPszProperty("pixel_shader");
	m_VertexShaderName = TreeNode.GetPszProperty("vertex_shader");
	m_PixelShader = CEngine::GetSingletonPtr()->getEffectsManager()->GetPixelShaderRef(m_PixelShaderName);
	m_VertexShader = CEngine::GetSingletonPtr()->getEffectsManager()->GetVertexShaderRef(m_VertexShaderName);

	m_GeometryShaderName = TreeNode.GetPszProperty("geometry_shader", "", false);
	m_GeometryShader = CEngine::GetSingletonPtr()->getEffectsManager()->GetGeometryShaderRef(m_GeometryShaderName);
}


CEffectTechnique::~CEffectTechnique()
{
}


CEffectVertexShader * CEffectTechnique::GetVertexShader()
{
	return m_VertexShader.getRef();
}


CEffectPixelShader * CEffectTechnique::GetPixelShader()
{
	return m_PixelShader.getRef();
}


CEffectGeometryShader * CEffectTechnique::GetGeometryShader()
{
	return m_GeometryShader.getRef();
}


void CEffectTechnique::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{
	if (!this || !m_VertexShader || !m_PixelShader) return;
	m_VertexShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
	m_PixelShader->SetConstantBuffer(IdBuffer, ConstantBuffer);
	if ( m_GeometryShader )
		m_GeometryShader->SetConstantBuffer( IdBuffer, ConstantBuffer );
}

void CEffectTechnique::destroy()
{
}
