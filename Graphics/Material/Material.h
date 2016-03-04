#ifndef MATERIAL_H
#define MATERIAL_H

#include <Base/Utils/Named.h>
#include <Base/Utils/Utils.h>
#include <Base/Math/Color.h>
#include <vector>

class CXMLTreeNode;
class CEffectTechnique;
class CTexture;
class CMaterialParameter;
class CRenderableObjectTechnique;
class CMaterialParameter;

class CMaterial : public CNamed
{
private:
	static int GetTextureStage(const std::string& type)
	{
		const std::string stages[] = {
			"albedo",
			"normal",
			"lightmap",
			"lightmap2",
			"lightmap3",
			"environment",
			"shadowmap"
		};
		int i = 0;
		for (auto const &t : stages)
		{
			if (t == type) return i;
			++i;
		}
		return -1;
	}

	std::vector<std::pair<uint8, CTexture *>> m_textures;
	std::vector<CMaterialParameter *> m_Parameters;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
public:
	CMaterial(CXMLTreeNode &TreeNode);
	virtual ~CMaterial();
	virtual void apply(CRenderableObjectTechnique *RenderableObjectTechnique = NULL);
	CRenderableObjectTechnique * getRenderableObjectTechique(){ return m_RenderableObjectTechnique; }
	std::vector<CMaterialParameter *> * getParameters(){ return &m_Parameters;  }
	void * GetNextParameterAddress(unsigned int NumBytes);
	void destroy();
};

#endif