#pragma once

#include <Base/Math/Math.h>
#include <Base/Utils/Named.h>
#include <Base/Utils/TMapContainer.h>

#include <Core/Component/Component.h>

#include <map>

class CXMLTreeNode;
class CCharacterControllerComponent;
class CPhysxComponent;
class CTriggerComponent;
class CFPSCameraComponent;
class CFreeCameraComponent;
class CRenderableComponent;
class CSpeakerComponent;
class CScriptedComponent;
class CAnimatedInstanceComponent;
class CScene;

class CElement : public CNamed
{
	Vect3f m_Position;
	Vect3f m_RotationYPR;
	Vect3f m_Scale;
	Mat44f m_TransformMatrix;
	bool m_TransformChanged;

	bool m_Enabled;

	CScene *m_Scene;

public:
	typedef TMapContainer<CComponent> ComponentContainer_t;

	CElement();
	CElement(const std::string& name);
	CElement(const CXMLTreeNode& node);
	~CElement();

	void Destroy();

	inline void SetPosition(const Vect3f &Position) { m_Position = Position; m_TransformChanged = true; }
	inline Vect3f GetPosition() const { return m_Position; }
	inline void SetYaw(float Yaw) { m_RotationYPR.x = Yaw; m_TransformChanged = true; }
	inline void SetPitch(float Pitch) { m_RotationYPR.y = Pitch; m_TransformChanged = true; }
	inline void SetRoll(float Roll) { m_RotationYPR.z = Roll; m_TransformChanged = true; }
	void SetQuat( Quatf quat );
	inline void SetYawPitchRoll(float Yaw, float Pitch, float Roll) { m_RotationYPR = Vect3f(Yaw, Pitch, Roll); m_TransformChanged = true; }
	inline float GetYaw() const { return m_RotationYPR.x; }
	inline float GetPitch() const { return m_RotationYPR.y; }
	inline float GetRoll() const { return m_RotationYPR.z; }
	inline Vect3f GetRotation() const { return m_RotationYPR; }
	inline void SetRotation(Vect3f rot) { m_RotationYPR = rot; m_TransformChanged = true; }
	inline void SetScale(const Vect3f &Scale) { m_Scale = Scale; m_TransformChanged = true; }
	inline Vect3f GetScale() const { return m_Scale; }

	const Mat44f & GetTransform();

	inline void SetEnabled(bool e) { m_Enabled = e; }
	inline bool GetEnabled() const { return m_Enabled; }

	void AddComponent(std::string Name, CComponent* component);

	void SendMsg(const std::string& message);

	void SendMsg( const std::string& message, CElement* arg1 );
	void SendMsg( const std::string& message, int arg1 );
	void SendMsg( const std::string& message, float arg1 );
	void SendMsg(const std::string& message, const std::string& arg1);
	void SendMsg( const std::string& message, int arg1, float arg2 );
	void SendMsg( const std::string& message, int arg1, const std::string& arg2 );

	CElement* Clone( const std::string& newName );

	CAnimatedInstanceComponent* GetAnimatedInstanceComponent();
	CCharacterControllerComponent* GetCharacterController();
	CPhysxComponent * GetPhysxComponent();
	CTriggerComponent * GetTriggerComponent();
	CFPSCameraComponent* GetCamera();
	CFreeCameraComponent* GetFreeCamera();
	CSpeakerComponent* GetSpeaker();
	CScriptedComponent* GetScript(const std::string& scriptName);

	inline void SetScene( CScene* scene ) { m_Scene = scene; }
	inline CScene* GetScene() const { return m_Scene; }

private:
	ComponentContainer_t m_componentContainer;

	template<typename T>
	void SendMessage_t(const std::string message, T arg1);

	template<typename... T>
	void SendMessage_t(const std::string message, T... arg1);
};

