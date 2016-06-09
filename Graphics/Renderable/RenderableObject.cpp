#include "RenderableObject.h"

#include "RenderableObjectTechnique.h"

#include "Mesh/RenderableVertexs.h"



CRenderableObject::CRenderableObject()
	: CNamed("")
{
}

CRenderableObject::CRenderableObject(CXMLTreeNode& treeNode)
	: C3DElement(treeNode)
	, CNamed(treeNode)
{
}
