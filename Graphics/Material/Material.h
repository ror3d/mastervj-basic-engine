#ifndef MATERIAL_H
#define MATERIAL_H

#include <Utils/Named.h>
#include <Base/Math/Color.h>
#include <vector>
#include "Context/ContextManager.h"

class CXMLTreeNode;
class CEffectTechnique;
class CTexture;

class CMaterial : public CNamed
{
private:
	std::vector<CTexture *> m_textures;
	CEffectTechnique *m_effectTechnique;

	float m_debugSize;
	CColor m_baseColor;
public:
	CMaterial(CXMLTreeNode &TreeNode);
	virtual ~CMaterial();

	void destroy();
	virtual void apply();

	CEffectTechnique* getEffectTechnique() const;

	void SetDebugSize(float _DebugSize) { m_debugSize = _DebugSize; }
	void SetBaseColor(const CColor& _BaseColor) { m_baseColor = _BaseColor; }

	virtual void SetShaderParameters(CContextManager* _Context) const
	{
		//_Context->SetDebugSize(m_debugSize);
		//_Context->SetBaseColor(m_baseColor);
	}

};

#endif