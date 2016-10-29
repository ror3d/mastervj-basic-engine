#ifndef COMPONENT_H
#define COMPONENT_H

#include "Utils/Named.h"
#include <vector>

class CElement;
class CContextManager;
class CXMLTreeNode;

class CComponent : public CNamed
{
public:
	struct Property
	{
		std::string name;
		std::string type;
		std::string value;
	};

	virtual std::string GetComponentType() = 0;

private:
	CElement* m_Owner;

	bool m_Initialized;

	bool m_Enabled;

protected:
	std::vector<Property> m_properties;

	bool m_Destroyed;

	virtual void Init() {}

	virtual void OnObjectInitialized() {};

public:
	CComponent(CXMLTreeNode& node, CElement* Owner);

	CComponent( const CComponent& base, CElement* Owner );

	virtual ~CComponent() {}

	void Initialize();

	void ObjectInitialized();

	virtual void PhysxUpdate() {}
	virtual void Update( double ElapsedTime ) {}
	virtual void FixedUpdate( double ElapsedTime ) {}
	virtual void Render(CContextManager&  _context) {}
	virtual void RenderDebug(CContextManager&  _context) {}

	virtual void SendMsg(const std::string&) {}

	virtual void SendMsg(const std::string&, const std::string&) {}

	virtual void SendMsg(const std::string&, int) {}

	virtual void SendMsg(const std::string&, float) {}

	virtual void SendMsg(const std::string&, CElement*) {}

	virtual void SendMsg(const std::string&, int, float) {}

	virtual void SendMsg(const std::string&, int, const std::string&) {}

	virtual void Destroy() = 0;

	virtual void SetNameFromParentName( const std::string& parentName );

	CElement* GetOwner()
	{
		return m_Owner;
	}

	inline void SetEnabled(bool en) { m_Enabled = en; }
	inline bool GetEnabled() const { return m_Enabled; }

	virtual CComponent* Clone(CElement* Owner) const = 0;
};

#endif
