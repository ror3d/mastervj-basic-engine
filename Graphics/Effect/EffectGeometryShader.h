#ifndef EFFECT_GEOMETRY_SHADER
#define EFFECT_GEOMETRY_SHADER

#include "Graphics/Effect/EffectShader.h"

class CEffectGeometryShader : public CEffectShader
{
protected:
	ID3D11GeometryShader *m_GeometryShader;
public:
	CEffectGeometryShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectGeometryShader();
	bool Load();
	ID3D11GeometryShader * GetGeometryShader() { return m_GeometryShader; }
	void destroy();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
};

#endif
