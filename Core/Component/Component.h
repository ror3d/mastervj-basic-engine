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
private:
	CElement* m_Owner;

	bool m_Initiaized;

	bool m_Enabled;

protected:
	std::vector<Property> m_properties;

	virtual void Init() {}

	virtual void OnObjectInitialized() {};

public:
	CComponent(CXMLTreeNode& node, CElement* Owner);

	CComponent( const std::string& Name, CElement* Owner );

	virtual ~CComponent() {}

	void Initialize();

	void ObjectInitialized();

	virtual void Update( float ElapsedTime ) {}
	virtual void FixedUpdate( float ElapsedTime ) {}
	virtual void Render(CContextManager&  _context) {}
	virtual void RenderDebug(CContextManager&  _context) {}

	virtual void SendMsg(const std::string) {}

	virtual void Destroy() = 0;

	CElement* GetOwner()
	{
		return m_Owner;
	}

	inline void SetEnabled(bool en) { m_Enabled = en; }
	inline bool GetEnabled() const { return m_Enabled; }
};

#endif
