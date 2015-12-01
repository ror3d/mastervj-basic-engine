#ifndef MATERIAL_H
#define MATERIAL_H

#include <Utils/Named.h>
#include <Utils/Utils.h>
#include <vector>
#include "Texture/Texture.h"
#include "Effect/EffectTechnique.h"
#include "Context/ContextManager.h"

class CXMLTreeNode;

class CMaterial : public CNamed
{
private:
	std::vector<CTexture *> m_textures;
	CEffectTechnique *m_effectTechnique;

	CContextManager::ERasterizerState m_rasterizerState;
	CContextManager::EDepthStencilState m_depthStencilState;
	CContextManager::EBlendState m_blendState;

	float m_debugSize;
	CColor m_baseColor;
public:
	CMaterial(const std::string &Filename);
	CMaterial(CContextManager::ERasterizerState _RasterizerState, CContextManager::EDepthStencilState _DepthStencilState, CContextManager::EBlendState _BlendState)
		: m_rasterizerState(_RasterizerState)
		, m_depthStencilState(_DepthStencilState)
		, m_blendState(_BlendState)
		, m_debugSize(1)
		, m_baseColor(1, 1, 1, 1)
	{}
	CMaterial(CXMLTreeNode &TreeNode);
	virtual ~CMaterial();

	void destroy();
	virtual void apply();

	CEffectTechnique* getEffectTechnique() const;

	void SetDebugSize(float _DebugSize) { m_debugSize = _DebugSize; }
	void SetBaseColor(const CColor& _BaseColor) { m_baseColor = _BaseColor; }

	virtual void SetShaderParameters(CContextManager* _Context) const
	{
		_Context->SetDebugSize(m_debugSize);
		_Context->SetBaseColor(m_baseColor);
	}

	bool HasBlending() const
	{
		switch (m_blendState)
		{
		case CContextManager::BLEND_SOLID:
		default:
			return false;
		case CContextManager::BLEND_CLASSIC:
		case CContextManager::BLEND_PREMULT:
			return true;
		}
		// TODO: Copiar el siguiente comentario dentro de CContextManager::EBlendState
		// NOTA: Si se añaden más estados aqui, modificar CMaterial::HasBlending
	}

	CContextManager::ERasterizerState GetRasterizerState() const { return m_rasterizerState; }
	CContextManager::EDepthStencilState GetDepthStencilState() const { return m_depthStencilState; }
	CContextManager::EBlendState GetBlendState() const { return m_blendState; }

};

#endif