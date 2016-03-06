#ifndef IA_MANAGER_H
#define IA_MANAGER_H

class CIAInstance;

class CIAManager
{
protected:
	CIAInstance * iaTurret;
	float m_actualTime;

public:
	CIAManager();
	virtual ~CIAManager();
	void Create();
	
	void Update(float elapsedTime);
	
};

#endif
