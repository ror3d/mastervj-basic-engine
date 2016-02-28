#ifndef APPLICATION_H
#define APPLICATION_H

#include "HelperTypes.h"
#include <Material/MaterialManager.h>

class CContextManager;
class CRenderManager;

class CApplication
{
public:
	CApplication( CContextManager *_ContextManager, CRenderManager *_renderManager );
	~CApplication();

	CRenderManager *m_RenderManager;

	void CreateCharController();
	void InitPhysx();
	
	void Update(float _ElapsedTime);
	void Render();
	bool sendedInfoToAnim;
	bool activeMovAnim;

	void Destroy() {/* TODO */}
private:
	CContextManager *m_ContextManager;
};

#endif
