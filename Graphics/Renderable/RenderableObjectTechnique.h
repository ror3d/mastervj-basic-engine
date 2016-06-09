#ifndef RENDERABLE_OBJECT_TECHNIQUE_H
#define RENDERABLE_OBJECT_TECHNIQUE_H

#include <Base/Utils/Named.h>
#include <Base/Utils/TMapManager.h>
class CEffectTechnique;

class CRenderableObjectTechnique : public CNamed
{
private:
	TMapManager<CEffectTechnique>::Ref m_EffectTechnique;
public:
	CRenderableObjectTechnique(const std::string &Name, TMapManager<CEffectTechnique>::Ref EffectTechnique);
	virtual ~CRenderableObjectTechnique() {}

	void SetEffectTechnique(TMapManager<CEffectTechnique>::Ref EffectTechnique);
	CEffectTechnique * GetEffectTechnique() { return m_EffectTechnique.getRef(); }
};

#endif