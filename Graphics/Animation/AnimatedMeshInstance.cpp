#include "AnimatedMeshInstance.h"
#include "AnimatedMesh.h"

#include "AnimatedMeshManager.h"

#include <Core/Engine/Engine.h>

#include <cal3d/cal3d.h>


CAnimatedMeshInstance::CAnimatedMeshInstance(CAnimatedMesh* animatedMesh)
	: m_AnimatedMesh(animatedMesh)
{
	m_CalModel = new CalModel(m_AnimatedMesh->GetCoreModel());
}


CAnimatedMeshInstance::~CAnimatedMeshInstance()
{
	delete m_CalModel;
}

void CAnimatedMeshInstance::Render(CContextManager *_context)
{
	m_AnimatedMesh->Render(_context, m_CalModel);
}

void CAnimatedMeshInstance::Update(float ElapsedTime)
{
	m_CalModel->update(ElapsedTime);
}

void CAnimatedMeshInstance::ExecuteAction(int Id, float DelayIn, float DelayOut, float
										  WeightTarget, bool AutoLock)
{
	m_CalModel->getMixer()->executeAction(Id, DelayIn, DelayOut, WeightTarget, AutoLock);
}

void CAnimatedMeshInstance::BlendCycle(int Id, float Weight, float DelayIn)
{
	m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}

void CAnimatedMeshInstance::ClearCycle(int Id, float DelayOut)
{
	m_CalModel->getMixer()->clearCycle(Id, DelayOut);
}

bool CAnimatedMeshInstance::IsCycleAnimationActive(int Id) const
{
	// TODO
	return false;
}

bool CAnimatedMeshInstance::IsActionAnimationActive(int Id) const
{
	// TODO
	return false;
}

void CAnimatedMeshInstance::SetAnimationTime(float animationTime) const
{
	//TODO: Not working
	//m_CalModel->getMixer()->setTimeFactor(animationTime);
	//m_CalModel->getMixer()->setAnimationTime(animationTime);
}

