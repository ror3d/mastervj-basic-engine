#pragma once

#include "VertexTypes.h"


class CRenderManager;
class CXMLTreeNode;

class C3DElement
{
protected:
		Vect3f m_Position, m_PrevPos;
		float m_Yaw, m_Pitch, m_Roll;
		bool m_TranslationUpdate, m_RotationUpdate, m_ScaleUpdate;
		Mat44f m_TransformMatrix, m_RotationMatrix, m_TranslationMatrix, m_ScaleMatrix;
		Vect3f m_Scale;
public:
		C3DElement();
		C3DElement( const Vect3f &Position); C3DElement( const Vect3f &Position, float Yaw, float Pitch, float Roll);
		C3DElement( float Yaw, float Pitch, float Roll);
		C3DElement( const CXMLTreeNode &XMLTreeNode);
		virtual ~C3DElement();

		virtual void SetPosition( const Vect3f &Position);
		inline const Vect3f & GetPosition() const {}
		inline const Vect3f & GetPrevPosition() const {}
		float GetYaw() const ;
		float GetPitch() const ;
		float GetRoll() const ;
		virtual void SetYaw( float Yaw);
		virtual void SetPitch( float Pitch);
		virtual void SetRoll( float Roll);
		virtual void SetYawPitchRoll( float Yaw, float Pitch, float Roll);
		virtual void SetScale( const Vect3f &Scale);
		inline const Vect3f & GetScale() const {}
		virtual void Render(CRenderManager *RenderManager);
		const Mat44f & GetTransform();
};

