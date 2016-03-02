#include "Material/MaterialParameter.h"

CMaterialParameter::CMaterialParameter(CXMLTreeNode &TreeNode, CMaterialParameter::TMaterialType MaterialType) : CNamed("")
	, m_MaterialType(MaterialType)
{
	setName(TreeNode.GetPszProperty("name"));
	m_paramValues = TreeNode.GetPszProperty("varParams","");
}

CMaterialParameter::~CMaterialParameter(){

}
