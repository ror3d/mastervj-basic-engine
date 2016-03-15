#include "Renderable/RenderableObjectTechnique.h"
#include "Effect/EffectTechnique.h"
#include <Utils/TMapManager.h>

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, TMapManager<CEffectTechnique>::Ref EffectTechnique)
	: CNamed(Name)
{
	m_EffectTechnique = EffectTechnique;
}

void CRenderableObjectTechnique::SetEffectTechnique(TMapManager<CEffectTechnique>::Ref EffectTechnique)
{
	m_EffectTechnique = EffectTechnique;
}