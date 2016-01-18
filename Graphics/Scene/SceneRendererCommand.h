#ifndef SCENE_RENDERER_COMMAND_H
#define SCENE_RENDERER_COMMAND_H

#include "Utils\Named.h"
#include "Material\Material.h"
#include "Context\ContextManager.h"

class CSceneRendererCommand : public CActive, public CNamed
{
protected:
	CMaterial *m_Material;
public:
	CSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CSceneRendererCommand();
	virtual void Execute(CContextManager &_context) = 0;
};

#endif