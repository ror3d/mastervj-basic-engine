#include "RenderableObject.h"

#include "RenderableObjectTechnique.h"

#include "Mesh/RenderableVertexs.h"

#include <Core/Component/ScriptedComponent.h>

void CRenderableObject::AddComponent(std::string Name, CScriptedComponent * component)
{
	m_componentManager->AddComponent(Name, component);
}