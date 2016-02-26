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

	void Init();
	
	void Update(float _ElapsedTime);
	void Render();

	void Destroy() {/* TODO */}
private:
	CContextManager *m_ContextManager;
	float m_Timer;
};

#endif
