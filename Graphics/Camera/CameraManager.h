#ifndef INC_CAMERAMANAGER_H_
#define INC_CAMERAMANAGER_H_


#include <Base/Utils/TMapManager.h>

#include "Frustum.h"

class CCamera;
class ICameraController;
class CContextManager;

class CCameraManager : public TMapManager<ICameraController>
{
protected:

	std::string m_currentCameraControllerName;

	CFrustum m_CullFrustum;

	CContextManager * m_context;
public:
	CCameraManager(CContextManager* context);
	virtual ~CCameraManager();

	void Load(const std::string& file);

	void Update( float ElapsedTime );

	void SetMatrixes();

	inline void SetCurrentCameraController( const std::string& name ) { m_currentCameraControllerName = name; }
	inline std::string GetCurrentCameraControllerName() { return m_currentCameraControllerName; }

	ICameraController* GetCurrentCameraController() { return get_internal( m_currentCameraControllerName ); }
};

#endif
