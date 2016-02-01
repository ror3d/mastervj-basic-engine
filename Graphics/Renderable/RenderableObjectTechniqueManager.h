#ifndef RENDERABLE_OBJECT_TECHNIQUE_MANAGER
#define RENDERABLE_OBJECT_TECHNIQUE_MANAGER

#include <Utils\TMapManager.h>
#include "Renderable\RenderableObjectTechnique.h"
#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Engine\Engine.h"

class CRenderableObjectTechniqueManager : public
	TMapManager<CRenderableObjectTechnique>
{
private:
	TMapManager<CPoolRenderableObjectTechnique*>
		m_PoolRenderableObjectTechniques;
	std::string m_Filename;
public:
	CRenderableObjectTechniqueManager();
	virtual ~CRenderableObjectTechniqueManager();
	bool InsertRenderableObjectTechnique(CPoolRenderableObjectTechnique	*PoolRenderableObjectTechniques, 
		const std::string &RenderableObjectTechniqueName,
		const std::string &TechniqueName);
	void Destroy();
	bool Load(const std::string &Filename);
	bool Reload();
	TMapManager<CPoolRenderableObjectTechnique*> & GetPoolRenderableObjectTechniques();
};

#endif