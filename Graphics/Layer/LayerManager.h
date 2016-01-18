#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

#include <string>
#include "Utils\TMapManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Context\ContextManager.h"

class CLayerManager : public TMapManager<CRenderableObjectsManager>
{
private:
	std::string
		m_Filename;
	CRenderableObjectsManager *m_DefaultLayer;
	CRenderableObjectsManager * GetLayer(CXMLTreeNode &Node);
	CRenderableObjectsManager * AddLayer(CXMLTreeNode &TreeNode);
public:
	CLayerManager();
	virtual ~CLayerManager();
	void Destroy();
	void Load(const std::string &Filename);
	void Reload();
	void Update(float ElapsedTime);
	void Render(CContextManager &RenderManager);
	void Render(CContextManager &RenderManager, const std::string &LayerName);
};

#endif