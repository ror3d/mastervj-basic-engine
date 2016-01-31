#ifndef MATERIAL_H
#define MATERIAL_H

#include <Base/Utils/Named.h>
#include <Base\Utils\Utils.h>
#include <Base/Math/Color.h>
#include <vector>
#include "Context/ContextManager.h"
#include "MaterialParameter.h"
#include "Graphics\Renderable\RenderableObjectTechnique.h"
#include "Effect/EffectTechnique.h"
#include "Engine/Engine.h"
#include "Texture/Texture.h"

class CXMLTreeNode;
class CEffectTechnique;
class CTexture;

class CMaterial : public CNamed
{
private:
	std::vector<CTexture *> m_textures;
	std::vector<CMaterialParameter *> m_Parameters;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
	void destroy();
public:
	CMaterial(CXMLTreeNode &TreeNode);
	virtual ~CMaterial();
	virtual void apply(CRenderableObjectTechnique *RenderableObjectTechnique = NULL);
	CRenderableObjectTechnique * getRenderableObjectTechique(){ return m_RenderableObjectTechnique; }
	std::vector<CMaterialParameter *> getParameters(){ return m_Parameters;  }
	void * GetNextParameterAddress(unsigned int NumBytes);
};

#endif