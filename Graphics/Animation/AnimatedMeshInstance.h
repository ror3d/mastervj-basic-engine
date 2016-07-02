#pragma once

#include <Graphics/Renderable/Renderable.h>

class CalModel;
class CAnimatedMesh;

class CAnimatedMeshInstance : public IRenderable
{
	CAnimatedMesh *m_AnimatedMesh;
	CalModel *m_CalModel;
public:
	CAnimatedMeshInstance(CAnimatedMesh* animatedMesh);
	~CAnimatedMeshInstance();

	void Render (CContextManager *_context);

	void Update(float ElapsedTime);

	void ExecuteAction(int Id, float DelayIn, float DelayOut, float
					   WeightTarget = 1.0f, bool AutoLock = true);
	void BlendCycle(int Id, float Weight, float DelayIn);
	void ClearCycle(int Id, float DelayOut);
	bool IsCycleAnimationActive(int Id) const;
	bool IsActionAnimationActive(int Id) const;
};

