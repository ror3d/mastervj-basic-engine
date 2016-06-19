#ifndef C3D_ELEMENT_H
#define C3D_ELEMENT_H

#include <Base/Math/Math.h>

class CXMLTreeNode;
class CContextManager;

class C3DElement
{
protected:
	Vect3f m_Position, m_PrevPos;
	float m_Yaw, m_Pitch, m_Roll;
	bool m_TranslationUpdate, m_RotationUpdate, m_ScaleUpdate;
	Mat44f m_TransformMatrix, m_RotationMatrix, m_TranslationMatrix, m_ScaleMatrix;
	Vect3f m_Scale;
	bool m_Visible;
	Quatf m_quat;
	bool m_isQuat;
public:
	C3DElement();
	virtual ~C3DElement();
	C3DElement(const Vect3f &Position);
	C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll);
	C3DElement(float Yaw, float Pitch, float Roll);
	C3DElement(const CXMLTreeNode &XMLTreeNode);
	virtual void SetPosition(const Vect3f &Position);
	Vect3f GetPosition() const;
	Vect3f GetPrevPosition() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;
	virtual void SetYaw(float Yaw);
	virtual void SetPitch(float Pitch);
	virtual void SetRoll(float Roll);
	virtual void SetYawPitchRoll(float Yaw, float Pitch, float Roll);
	virtual void SetScale(const Vect3f &Scale);
	const Vect3f & GetScale() const;
	const Mat44f & GetTransform();
	bool GetVisible();
	virtual void SetVisible(bool Visible);
	//virtual void Render(CContextManager *_context) = 0; //TODO
	void Render(CContextManager *_context);
	Quatf GetQuat() const { return m_quat; }
	void SetQuat(const Quatf &q) {
		m_quat = q;
		m_isQuat = true;
		m_RotationUpdate = true;
	}
};

#endif
