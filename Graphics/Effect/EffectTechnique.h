#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H


#include <Base/Utils/Named.h>
#include <Base/Utils/TMapManager.h>
class CEffectVertexShader;
class CEffectPixelShader;
class CEffectGeometryShader;

class CEffectTechnique : public CNamed
{
private:
	TMapManager<CEffectVertexShader>::Ref m_VertexShader;
	TMapManager<CEffectPixelShader>::Ref m_PixelShader;
	TMapManager<CEffectGeometryShader>::Ref m_GeometryShader;
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
	std::string m_GeometryShaderName;
public:
	CEffectTechnique(CXMLTreeNode &TreeNode);
	virtual ~CEffectTechnique();
	CEffectVertexShader * GetVertexShader();
	CEffectPixelShader * GetPixelShader();
	CEffectGeometryShader * GetGeometryShader();

	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
	void destroy();
};

#endif
