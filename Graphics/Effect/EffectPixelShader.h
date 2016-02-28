#ifndef EFFECT_PIXEL_SHADER
#define EFFECT_PIXEL_SHADER

#include "Effect/EffectShader.h"

class CEffectPixelShader : public CEffectShader
{
protected:
	ID3D11PixelShader *m_PixelShader;
public:
	CEffectPixelShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectPixelShader();
	bool Load();
	ID3D11PixelShader * GetPixelShader() { return m_PixelShader; }
	void destroy();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
};

#endif
