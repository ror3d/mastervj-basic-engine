#include "CinematicsAction.h"

CCinematicsAction::CCinematicsAction(CXMLTreeNode node)
{
	m_Time = node.GetFloatProperty("time", 0.0);

}

CCinematicsAction::~CCinematicsAction()
{

}
