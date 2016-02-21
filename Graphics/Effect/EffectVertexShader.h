#ifndef EFFECT_VERTEX_SHADER
#define EFFECT_VERTEX_SHADER

#include "EffectShader.h"
#include <d3d11.h>

class CEffectVertexShader : public CEffectShader
{
protected:
	ID3D11VertexShader *m_VertexShader;
	ID3D11InputLayout *m_VertexLayout;
	std::string m_VertexType;
public:
	CEffectVertexShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectVertexShader();
	bool Load();
	ID3D11VertexShader * GetVertexShader() { return m_VertexShader; }
	ID3D11InputLayout * GetVertexLayout() { return m_VertexLayout; }
	void destroy();

	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
};

#endif
