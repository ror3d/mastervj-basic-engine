#pragma once
#include "Component.h"

class CAnimatedMeshInstance;
class CAnimatedInstanceComponent :
	public CComponent
{
	CAnimatedMeshInstance *m_AnimatedMeshInstance;
	std::vector<std::string> m_layers;
public:
	CAnimatedInstanceComponent(const std::string& name, CElement* Owner);
	CAnimatedInstanceComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	~CAnimatedInstanceComponent();

	void Update(float ElapsedTime);
	void Render(CContextManager&  _context);
	void Destroy();

	void ExecuteAction(int Id, float DelayIn, float DelayOut, float
					   WeightTarget = 1.0f, bool AutoLock = true);
	void BlendCycle(int Id, float Weight, float DelayIn);
	void ClearCycle(int Id, float DelayOut);
	bool IsCycleAnimationActive(int Id) const;
	bool IsActionAnimationActive(int Id) const;
};

