#ifndef RENDERABLE_OBJECT_TECHNIQUE_MANAGER_H
#define RENDERABLE_OBJECT_TECHNIQUE_MANAGER_H

#include <Utils\TMapManager.h>

class CRenderableObjectTechnique;
class CPoolRenderableObjectTechnique;

class CRenderableObjectTechniqueManager : public TMapManager<CRenderableObjectTechnique>
{
private:
	TMapManager<CPoolRenderableObjectTechnique> m_PoolRenderableObjectTechniques;
	std::string m_Filename;
public:
	CRenderableObjectTechniqueManager();
	virtual ~CRenderableObjectTechniqueManager();
	bool InsertRenderableObjectTechnique(TMapManager<CPoolRenderableObjectTechnique>::Ref PoolRenderableObjectTechniques,
		const std::string &RenderableObjectTechniqueName,
		const std::string &TechniqueName);
	void destroy();
	bool Load(const std::string &Filename);
	bool Reload();
	TMapManager<CPoolRenderableObjectTechnique> & GetPoolRenderableObjectTechniques();
	CPoolRenderableObjectTechnique* getPool(const std::string& pool);
};

#endif
