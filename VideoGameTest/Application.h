#ifndef APPLICATION_H
#define APPLICATION_H

#include "HelperTypes.h"
#include <Material/MaterialManager.h>
#include <Engine\Engine.h>

class CContextManager;
class CRenderManager;

class CApplication
{
public:
	CApplication( CContextManager *_ContextManager, CRenderManager *_renderManager );
	~CApplication();

	CRenderManager *m_RenderManager;

	void Init();
	
	void Update(float _ElapsedTime);
	void Render();

	void Destroy() {/* TODO */}
private:
	CContextManager *m_ContextManager;
	

	CColor m_BackgroundColor;
};

#endif
