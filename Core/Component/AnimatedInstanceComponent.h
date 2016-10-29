#pragma once
#include "Component.h"

class CAnimatedMeshInstance;
class CAnimatedInstanceComponent :
	public CComponent
{
	std::string m_animatedMeshCore;
	CAnimatedMeshInstance *m_AnimatedMeshInstance;
	std::vector<std::string> m_layers;
public:
	CAnimatedInstanceComponent(const CAnimatedInstanceComponent& base, CElement* Owner);
	CAnimatedInstanceComponent(CXMLTreeNode& node, CElement* Owner);
	~CAnimatedInstanceComponent();

	void Update(double ElapsedTime);
	void Render(CContextManager&  _context);
	void Destroy();

	void ExecuteAction(int Id, float DelayIn, float DelayOut, float
					   WeightTarget = 1.0f, bool AutoLock = true);
	void BlendCycle(int Id, float Weight, float DelayIn);
	void ClearCycle(int Id, float DelayOut);
	bool IsCycleAnimationActive(int Id) const;
	bool IsActionAnimationActive(int Id) const;
	void SetAnimationTime(float animationTime) const;

	static const std::string COMPONENT_TYPE;
	virtual std::string GetComponentType() { return COMPONENT_TYPE; }


	virtual CComponent* Clone(CElement* Owner) const { return new CAnimatedInstanceComponent( *this, Owner ); }
};
