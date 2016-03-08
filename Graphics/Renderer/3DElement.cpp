#include "3DElement.h"
#include <Engine/Engine.h>


C3DElement::C3DElement()
	: m_Position(0.0f, 0.0f, 0.0f)
	, m_PrevPos(0.0f, 0.0f, 0.0f)
	, m_Yaw(0.0f)
	, m_Pitch(0.0f)
	, m_Roll(0.0f)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_Visible(true)
	, m_TranslationUpdate(true)
	, m_RotationUpdate(true)
	, m_ScaleUpdate(true)
{
}

C3DElement::C3DElement(const Vect3f &Position)
	: m_Position(Position)
	, m_PrevPos(0.0f, 0.0f, 0.0f)
	, m_Yaw(0.0f)
	, m_Pitch(0.0f)
	, m_Roll(0.0f)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_TranslationUpdate(true)
	, m_RotationUpdate(true)
	, m_ScaleUpdate(true)
	, m_Visible(true)
{
}


C3DElement::C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll)
	: m_Position(Position)
	, m_PrevPos(0.0f, 0.0f, 0.0f)
	, m_Yaw(Yaw)
	, m_Pitch(Pitch)
	, m_Roll(Roll)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_TranslationUpdate(true)
	, m_RotationUpdate(true)
	, m_ScaleUpdate(true)
	, m_Visible(true)
{
}


C3DElement::C3DElement(float Yaw, float Pitch, float Roll)
	: m_Yaw(Yaw)
	, m_Pitch(Pitch)
	, m_Roll(Roll)
	, m_PrevPos(0.0f, 0.0f, 0.0f)
	, m_Scale(1.0f, 1.0f, 1.0f)
	, m_TranslationUpdate(true)
	, m_RotationUpdate(true)
	, m_ScaleUpdate(true)
	, m_Visible(true)
{
}


C3DElement::C3DElement(const CXMLTreeNode &treeNode)
	: m_TranslationUpdate(true)
	, m_RotationUpdate(true)
	, m_ScaleUpdate(true)
	, m_PrevPos(0.0f, 0.0f, 0.0f)
	, m_Yaw(0.0f)
	, m_Pitch(0.0f)
	, m_Roll(0.0f)
	, m_Scale(1.0f, 1.0f, 1.0f)
{
	m_Position = treeNode.GetVect3fProperty( "pos" , Vect3f(0, 0, 0), false);
	m_Scale = treeNode.GetVect3fProperty( "scale" , Vect3f(1, 1, 1), false);
	if ( treeNode.GetPszProperty( "rotation", nullptr, false ) != nullptr )
	{
		Vect3f rot = treeNode.GetVect3fProperty( "rotation", Vect3f( 0, 0, 0 ), false );
		m_Yaw = rot.x;
		m_Pitch = rot.y;
		m_Roll = rot.z;
	}
	else
	{
		m_Yaw = treeNode.GetFloatProperty( "yaw" );
		m_Pitch = treeNode.GetFloatProperty( "pitch" );
		m_Roll = treeNode.GetFloatProperty( "roll" );
	}
	m_Yaw = mathUtils::Deg2Rad(m_Yaw);
	m_Pitch = mathUtils::Deg2Rad(m_Pitch);
	m_Roll = mathUtils::Deg2Rad(m_Roll);
	m_Visible = treeNode.GetBoolProperty( "visible", true, false );
}


C3DElement::~C3DElement()
{
}


void C3DElement::SetPosition(const Vect3f &Position)
{
	m_Position = Position;
	m_TranslationUpdate = true;
}


const Vect3f & C3DElement::GetPosition() const
{
	return m_Position;
}


const Vect3f & C3DElement::GetPrevPosition() const
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
	m_RotationUpdate = true;
}


void C3DElement::SetPitch(float Pitch)
{
	m_Pitch = Pitch;
	m_RotationUpdate = true;
}


void C3DElement::SetRoll(float Roll)
{
	m_Roll = Roll;
	m_RotationUpdate = true;
}


void C3DElement::SetYawPitchRoll(float Yaw, float Pitch, float Roll)
{
	m_Yaw = Yaw;
	m_Pitch = Pitch;
	m_Roll = Roll;
	m_RotationUpdate = true;
}


void C3DElement::SetScale(const Vect3f &Scale)
{
	m_Scale = Scale;
	m_ScaleUpdate = true;
}


const Vect3f & C3DElement::GetScale() const
{
	return m_Scale;
}


const Mat44f & C3DElement::GetTransform()
{
	if ( m_RotationUpdate || m_TranslationUpdate || m_ScaleUpdate )
	{
		if ( m_RotationUpdate )
		{
			m_RotationMatrix.SetIdentity();

			Mat44f l_RotX;
			l_RotX.SetIdentity();
			l_RotX.RotByAngleX(m_Roll);
			Mat44f l_RotY;
			l_RotY.SetIdentity();
			l_RotY.RotByAngleY(m_Yaw);
			Mat44f l_RotZ;
			l_RotZ.SetIdentity();
			l_RotZ.RotByAngleZ(m_Pitch);

			m_RotationMatrix = l_RotX*l_RotZ*l_RotY;

			m_RotationUpdate = false;
		}
		if ( m_TranslationUpdate )
		{
			m_TranslationMatrix.SetFromPos( m_Position );
			m_TranslationUpdate = false;
		}
		if ( m_ScaleUpdate )
		{
			m_ScaleMatrix.SetFromScale( m_Scale.x, m_Scale.y, m_Scale.z );
			m_ScaleUpdate = false;
		}
		m_TransformMatrix = m_ScaleMatrix * m_RotationMatrix * m_TranslationMatrix;
	}
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
