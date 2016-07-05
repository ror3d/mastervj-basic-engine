#include "Named.h"
#include <XML/XMLTreeNode.h>


CNamed::CNamed(const CXMLTreeNode &node)
{
	m_name = node.GetPszProperty("name", "", false);
}

CNamed::CNamed(const std::string &name)
	: m_name(name)
{
}


CNamed::~CNamed()
{
}


void CNamed::setName(const std::string &name)
{
	m_name = name;
}

std::string CNamed::getName() const
{
	return m_name;
}
