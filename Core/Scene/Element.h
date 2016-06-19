#pragma once

#include <Base/Utils/Named.h>
#include <Base/Utils/TMapContainer.h>
#include <Base/Math/Math.h>

#include <Core/Component/Component.h>

#include <map>

class CXMLTreeNode;
class CCharacterControllerComponent;
class CPhysxComponent;
class CTriggerComponent;
class CFPSCameraComponent;
class CRenderableComponent;
class CSpeakerComponent;
class CAnimatedInstanceComponent;

class CElement : public CNamed
{
	Vect3f m_Position;
	Vect3f m_RotationYPR;
	Vect3f m_Scale;
	Mat44f m_TransformMatrix;
	bool m_TransformChanged;

	bool m_Enabled;

public:
	typedef TMapContainer<CComponent> ComponentContainer_t;

	CElement();
	CElement(const std::string& name);
	CElement(const CXMLTreeNode& node);
	~CElement();

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
	inline void SetScale(const Vect3f &Scale) { m_Scale = Scale; m_TransformChanged = true; }
	inline Vect3f GetScale() const { return m_Scale; }

	const Mat44f & GetTransform();

	inline void SetEnabled(bool e) { m_Enabled = e; }
	inline bool GetEnabled() const { return m_Enabled; }

	void AddComponent(std::string Name, CComponent* component);

	void SendMsg(const std::string message);

	template<typename T>
	void SendMsg(const std::string message, T arg1);

	CAnimatedInstanceComponent* GetAnimatedInstanceComponent();
	CCharacterControllerComponent* GetCharacterController();
	CPhysxComponent * GetPhysxComponent();
	CTriggerComponent * GetTriggerComponent();
	CFPSCameraComponent* GetCamera();
	CSpeakerComponent* GetSpeaker();

private:
	ComponentContainer_t m_componentContainer;
};


template<typename T>
void CElement::SendMsg(const std::string msg, T arg1)
{
	for (auto &const c : m_componentContainer)
	{
		c.second->SendMsg(msg, arg1);
	}
}
