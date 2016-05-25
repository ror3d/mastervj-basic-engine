#pragma once

#include <string>

class CXMLTreeNode;
class CNamed
{
private:
	std::string m_name;

protected:
	CNamed() {}

public:
	CNamed(const CXMLTreeNode &node);
	CNamed(const std::string &name);
	virtual ~CNamed();

	virtual void setName(const std::string &name);
	virtual const std::string &getName() const;
};

