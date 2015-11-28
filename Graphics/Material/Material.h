#pragma once

#include "Context/ContextManager.h"

class CMaterial
{
public:
	CMaterial(CContextManager::ERasterizerState _RasterizerState, CContextManager::EDepthStencilState _DepthStencilState, CContextManager::EBlendState _BlendState)
		: m_RasterizerState(_RasterizerState)
		, m_DepthStencilState(_DepthStencilState)
		, m_BlendState(_BlendState)
		, m_DebugSize(1)
		, m_BaseColor(1,1,1,1)
	{}

	void SetDebugSize(float _DebugSize) { m_DebugSize = _DebugSize; }
	void SetBaseColor(const CColor& _BaseColor) { m_BaseColor = _BaseColor; }

	virtual void SetShaderParameters(CContextManager* _Context) const
	{
		_Context->SetDebugSize(m_DebugSize);
		_Context->SetBaseColor(m_BaseColor);
	}

	bool HasBlending() const
	{
		switch (m_BlendState)
		{
		case CContextManager::BLEND_SOLID:
		default:
			return false;
		case CContextManager::BLEND_CLASSIC:
		case CContextManager::BLEND_PREMULT:
			return true;
		}
		// TODO: Copiar el siguiente comentario dentro de CContextManager::EBlendState
		// NOTA: Si se añaden más estados aquí, modificar CMaterial::HasBlending
	}

	CContextManager::ERasterizerState GetRasterizerState() const { return m_RasterizerState; }
	CContextManager::EDepthStencilState GetDepthStencilState() const { return m_DepthStencilState; }
	CContextManager::EBlendState GetBlendState() const { return m_BlendState; }

private:
	CContextManager::ERasterizerState m_RasterizerState;
	CContextManager::EDepthStencilState m_DepthStencilState;
	CContextManager::EBlendState m_BlendState;

	float m_DebugSize;
	CColor m_BaseColor;
};
