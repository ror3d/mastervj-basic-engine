#ifndef RENDERABLE_OBJECTS_MANAGER_H
#define RENDERABLE_OBJECTS_MANAGER_H

#include <string>
#include "Utils/Active.h"
#include "Utils/TMapManager.h"
#include "Math/Math.h"

class CContextManager;
class CRenderableObject;
class CXMLTreeNode;

class CRenderableObjectsManager : public TMapManager<CRenderableObject>, public CActive
{
private:
public:
	CRenderableObjectsManager();
	~CRenderableObjectsManager();
	void Update(float ElapsedTime);
	void Render(CContextManager *_context);
	void AddMeshInstance(CXMLTreeNode &TreeNode);
	void AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position);
	void AddAnimatedInstanceModel(CXMLTreeNode &TreeNode);
	void AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position);
	void Load(const std::string &FileName);
};

#endif