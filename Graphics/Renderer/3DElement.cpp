#include "3DElement.h"
#include <Engine/Engine.h>


C3DElement::C3DElement()
	: m_Position(0.0f, 0.0f, 0.0f)
	, m_PrevPos(0.0f, 0.0f, 0.0f)
	, m_Yaw(0.0f)
	, m_Pitch(0.0f)
	, m_Roll(0.0f)
	, m_TranslationUpdate(false)
	, m_RotationUpdate(false)
	, m_ScaleUpdate(false)
	, m_TransformMatrix(0.0f, 0.0f, 0.0f)
	, m_RotationMatrix(0.0f, 0.0f, 0.0f)
	, m_TranslationMatrix(0.0f, 0.0f, 0.0f)
	, m_ScaleMatrix(0.0f, 0.0f, 0.0f)
	, m_Scale(0.0f, 0.0f, 0.0f)
	, m_Visible(true)
{
}


C3DElement::~C3DElement()
{
}

C3DElement::C3DElement(const Vect3f &Position)
	: m_Position(Position)
{
}


C3DElement::C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll)
	: m_Position(Position)
	, m_Yaw(Yaw)
	, m_Pitch(Pitch)
	, m_Roll(Roll)
{
}


C3DElement::C3DElement(float Yaw, float Pitch, float Roll)
	: m_Yaw(Yaw)
	, m_Pitch(Pitch)
	, m_Roll(Roll)
{
}


C3DElement::C3DElement(const CXMLTreeNode &XMLTreeNode)
{

}


void C3DElement::SetPosition(const Vect3f &Position)
{
	m_Position = Position;
}


inline const Vect3f & C3DElement::GetPosition() const
{
	return m_Position;
}


inline const Vect3f & C3DElement::GetPrevPosition() const
{
	return m_Position;
}


float C3DElement::GetYaw() const
{
	return m_Yaw;
}


float C3DElement::GetPitch() const
{
	return m_Pitch;
}


float C3DElement::GetRoll() const
{
	return m_Roll;
}


void C3DElement::SetYaw(float Yaw)
{
	m_Yaw = Yaw;
}


void C3DElement::SetPitch(float Pitch)
{
	m_Pitch = Pitch;
}


void C3DElement::SetRoll(float Roll)
{
	m_Roll = Roll;
}


void C3DElement::SetYawPitchRoll(float Yaw, float Pitch, float Roll)
{
	m_Yaw = Yaw;
	m_Pitch = Pitch;
	m_Roll = Roll;
}


void C3DElement::SetScale(const Vect3f &Scale)
{
	m_Scale = Scale;
}


inline const Vect3f & C3DElement::GetScale() const
{
	return m_Scale;
}


const Mat44f & C3DElement::GetTransform()
{
	return m_TransformMatrix;
}

bool C3DElement::GetVisible()
{
	return m_Visible;
}


void C3DElement::SetVisible(bool Visible)
{
	m_Visible = Visible;
}
