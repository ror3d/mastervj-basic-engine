#include "IAManager.h"
#include "IA/IAInstance.h"

CIAManager::CIAManager()
{
	m_actualTime = 0;
}

CIAManager::~CIAManager()
{
}

void CIAManager::Update(float elapsedTime)
{
	m_actualTime += elapsedTime;
	if (m_actualTime >= iaTurret->m_timeToUpdate){
		iaTurret->Update(elapsedTime);
		m_actualTime = 0;
	}
}

void CIAManager::Create(){
	iaTurret = new CIAInstance("Box001", 0.01f);
}
