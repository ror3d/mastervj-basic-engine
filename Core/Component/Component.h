#ifndef COMPONENT_H
#define COMPONENT_H

#include "Utils/Named.h"
#include <map>

class CRenderableObject;
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
	CRenderableObject* m_Owner;

	bool m_Initiaized;

	bool m_Enabled;

protected:
	std::vector<Property> m_properties;

	virtual void Init() {}

	virtual void OnObjectInitialized() {};

public:
	CComponent(CXMLTreeNode& node, CRenderableObject* Owner);

	CComponent( const std::string& Name, CRenderableObject* Owner );

	virtual ~CComponent() {}

	void Initialize();

	void ObjectInitialized();

	virtual void Update( float ElapsedTime ) {}
	virtual void FixedUpdate( float ElapsedTime ) {}
	virtual void Render(CContextManager&  _context) {}
	virtual void RenderDebug(CContextManager&  _context) {}

	virtual void SendMsg(const std::string) {}

	virtual void Destroy() = 0;

	CRenderableObject* GetOwner()
	{
		return m_Owner;
	}

	inline void SetEnabled(bool en) { m_Enabled = en; }
	inline bool GetEnabled() const { return m_Enabled; }
};

#endif
