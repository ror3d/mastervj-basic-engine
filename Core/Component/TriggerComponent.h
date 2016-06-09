#ifndef TRIGGER_COMPONENT_H
#define TRIGGER_COMPONENT_H

#include "Component.h"

#include <vector>

class CTriggerComponent : public CComponent
{
	std::string m_name;
	bool m_EnterTrigger;
	bool m_StayTrigger;
	bool m_ExitTrigger;

protected:
	virtual void Init();

public:
	CTriggerComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	CTriggerComponent(const std::string& name, CElement* Owner);
	virtual ~CTriggerComponent();

	virtual void FixedUpdate( float ElapsedTime );

	virtual void Destroy();

	void Activate();

	int GetStateTrigger(); //0 not activated, 1 enter, 2 stay, 3 exit
	void SetStateTrigger(int num);

	std::string GetTriggerLocalName(){ return m_name; }
};

#endif
