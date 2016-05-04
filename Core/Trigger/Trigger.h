#ifndef TRIGGER_H
#define TRIGGER_H

#include <Utils/Named.h>

class CXMLTreeNode;

class CTrigger : public CNamed
{

private:
	bool m_activated;
	bool m_once;
	std::string m_action; //0 Cinematics 1 Other
	std::string m_nameAction;
public:
	CTrigger(CXMLTreeNode Node);
	virtual ~CTrigger();
	void Activate();
	void LaunchAction();


};

#endif
