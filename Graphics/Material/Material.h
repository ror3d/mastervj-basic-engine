#ifndef MATERIAL_H
#define MATERIAL_H

#include <Base/Utils/Named.h>
#include <Base/Utils/Utils.h>
#include <Base/Math/Color.h>
#include <Base/Utils/TMapManager.h>
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

	std::vector<TMapManager<CTexture>::Ref> m_textures;
	std::vector<uint8> m_stages;
	std::vector<CMaterialParameter *> m_Parameters;
	TMapManager<CRenderableObjectTechnique>::Ref m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
public:
	CMaterial(CXMLTreeNode &TreeNode);
	virtual ~CMaterial();
	virtual void apply(CRenderableObjectTechnique *RenderableObjectTechnique = NULL);
	CRenderableObjectTechnique * getRenderableObjectTechique() { return m_RenderableObjectTechnique.getRef(); }
	std::vector<CMaterialParameter *> * getParameters(){ return &m_Parameters;  }
	void * GetNextParameterAddress(unsigned int NumBytes);
	void destroy();
};

#endif