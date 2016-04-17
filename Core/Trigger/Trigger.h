#ifndef TRIGGER_H
#define TRIGGER_H

#include "Utils/Named.h"
#include "XML/XMLTreeNode.h"

class CTrigger : public CNamed
{

private:
	bool m_activated;
	bool m_once;
	int m_action; //0 Cinematics 1 Other
	std::string m_nameAction;
public:
	CTrigger(CXMLTreeNode Node);
	virtual ~CTrigger();
	void Activate();
	void LaunchAction();


};

#endif
