#include "Material/MaterialParameter.h"
#include <Base/XML/XMLTreeNode.h>

CMaterialParameter::CMaterialParameter(CXMLTreeNode &TreeNode, CMaterialParameter::TMaterialType MaterialType) : CNamed("")
	, m_MaterialType(MaterialType)
{
	setName(TreeNode.GetPszProperty("name"));
	m_paramValues = TreeNode.GetPszProperty("varParams","");
}

CMaterialParameter::CMaterialParameter(const CMaterialParameter& param)
	: m_paramValues(param.m_paramValues)
	, m_MaterialType(param.m_MaterialType)
{
	setName(param.getName());
}

CMaterialParameter::~CMaterialParameter(){

}
