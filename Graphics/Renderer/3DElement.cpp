#include "3DElement.h"

#include "XML/XMLTreeNode.h"


C3DElement::C3DElement()
{
}


C3DElement::~C3DElement()
{
}

C3DElement::C3DElement( const Vect3f &Position) {}
C3DElement::C3DElement( const Vect3f &Position, float Yaw, float Pitch, float Roll) {}
C3DElement::C3DElement( float Yaw, float Pitch, float Roll) {}
C3DElement::C3DElement( const CXMLTreeNode &XMLTreeNode) {}

void C3DElement::SetPosition( const Vect3f &Position) {}
float C3DElement::GetYaw() const  { return 0; }
float C3DElement::GetPitch() const  { return 0; }
float C3DElement::GetRoll() const  { return 0; }
void C3DElement::SetYaw( float Yaw) {}
void C3DElement::SetPitch( float Pitch) {}
void C3DElement::SetRoll( float Roll) {}
void C3DElement::SetYawPitchRoll( float Yaw, float Pitch, float Roll) {}
void C3DElement::SetScale( const Vect3f &Scale) {}
void C3DElement::Render(CRenderManager *RenderManager) {}
const Mat44f & C3DElement::GetTransform() { return m_TransformMatrix;  }
