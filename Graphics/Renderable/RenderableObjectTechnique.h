#ifndef RENDERABLE_OBJECT_TECHNIQUE_H
#define RENDERABLE_OBJECT_TECHNIQUE_H

#include <Utils/Named.h>
class CEffectTechnique;

class CRenderableObjectTechnique : public CNamed
{
private:
	CEffectTechnique *m_EffectTechnique;
public:
	CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);
	virtual ~CRenderableObjectTechnique() {}

	void SetEffectTechnique(CEffectTechnique *EffectTechnique);
	CEffectTechnique * GetEffectTechnique() const { return m_EffectTechnique; }
};

#endif