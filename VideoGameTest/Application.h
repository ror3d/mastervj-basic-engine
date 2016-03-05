#ifndef APPLICATION_H
#define APPLICATION_H

#include "HelperTypes.h"
#include <Material/MaterialManager.h>

class CContextManager;
class CRenderManager;

class CApplication
{
public:
	CApplication( CContextManager *_ContextManager);
	~CApplication();


	void CreateCharController();
	void InitPhysx();
	void Init();
	
	void Update(float _ElapsedTime);
	void Render();
	bool activeMovAnim;

	void Destroy() {/* TODO */}
private:
	CContextManager *m_ContextManager;
	float m_Timer;
	bool m_FixedCamera;
};

#endif
