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

	void Update(double _ElapsedTime);
	void Render();
	bool activeMovAnim;

	void Destroy() {/* TODO */}
private:
	CContextManager *m_ContextManager;
	double m_Timer;
	double m_FixedTimer;
	bool m_FixedCamera;
};

#endif
