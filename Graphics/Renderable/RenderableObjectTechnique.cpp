#include "Renderable/RenderableObjectTechnique.h"
#include "Effect/EffectTechnique.h"

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, TMapManager<CEffectTechnique>::Ref EffectTechnique)
	: CNamed(Name)
{
	m_EffectTechnique = std::move(EffectTechnique);
}

void CRenderableObjectTechnique::SetEffectTechnique(TMapManager<CEffectTechnique>::Ref EffectTechnique)
{
	m_EffectTechnique = std::move(EffectTechnique);
}