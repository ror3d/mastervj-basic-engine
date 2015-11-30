#include "Utils/MapManager.h"
#include "RenderableObject.h"

class CRenderableObjectsManager : public TemplatedMapManager<CRenderableObject>
{
private:
public:
	CRenderableObjectsManager();
	~CRenderableObjectsManager();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	CRenderableObject * AddMeshInstance(CXMLTreeNode &TreeNode);
	CRenderableObject * AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position);
	CRenderableObject * AddAnimatedInstanceModel(CXMLTreeNode &TreeNode);
	CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position);
	void Load(const std::string &FileName);
};
