#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H


#include "Utils\Named.h"
#include "EffectPixelShader.h"
#include "EffectVertexShader.h"

class CEffectTechnique : public CNamed
{
private:
	CEffectVertexShader *m_VertexShader;
	CEffectPixelShader *m_PixelShader;
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
public:
	CEffectTechnique(CXMLTreeNode &TreeNode);
	virtual ~CEffectTechnique();
	CEffectVertexShader * GetVertexShader();
	CEffectPixelShader * GetPixelShader();
	void Refresh();
};

#endif
