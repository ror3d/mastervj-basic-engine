#include "RenderManager.h"

#include "Context/ContextManager.h"
#include "Material/MaterialManager.h"
#include "RenderableObjectsManager.h"
#include <Engine/Engine.h>


CRenderManager::CRenderManager()
	: m_UseDebugCamera(false)
{
	m_RenderableObjects = CEngine::GetSingleton().getRenderableObjectManager();
}


void CRenderManager::SetCurrentCamera(const CCamera& _CurrentCamera)
{
	m_CurrentCamera = _CurrentCamera;
	Mat44f viewProj = m_CurrentCamera.GetView() * m_CurrentCamera.GetProjection();
	m_CullFrustum.Update(viewProj);
}


void CRenderManager::Render(CContextManager* _Context, CMaterialManager* _MaterialManager)
{

	CCamera &l_Camera = (m_UseDebugCamera ? m_DebugCamera : m_CurrentCamera);
	/*if (m_UseDebugCamera)
	{
		m_DebugCamera.GetView()

		_Context->SetCamera(m_DebugCamera);
	}
	else
	{
		_Context->SetCamera(m_CurrentCamera);
	}*/
	CEffectManager::m_Parameters.m_View=l_Camera.GetView();
	CEffectManager::m_Parameters.m_Projection= l_Camera.GetProjection();
	/*
	static float r = 0;
	r += 0.01;
	m_CurrentCamera.SetLookAt(Vect3f(sinf(r), 0, cosf(r)));
	CEngine::GetSingleton().getEffectsManager()->m_Parameters.m_View = m_CurrentCamera.GetView();
	*/

	m_RenderableObjects->Render(this);



	// TODO crear un vector para objetos transparentes
	//struct BlendedSubmesh
	//{
	//    const CRenderableVertexs* vertices;
	//    Mat44f world;
	//    const CMaterial* material;
	//    const Vect3f& position;
	//};
	//std::vector<BlendedSubmesh> l_SubmeshesWithBlend;


	/*
	for (auto it = m_RenderableObjects->(); it != m_RenderableObjects->end(); ++it)
	{
		const CRenderableObject* l_RenderableObject = m_RenderableObjects[i];
		const SPositionOrientation& l_Transform = l_RenderableObject->GetTransform();

		Mat44f world;
		world.SetFromPosAndAnglesYXZ(l_Transform.Position, l_Transform.Yaw, l_Transform.Pitch, l_Transform.Roll);
		_Context->SetWorldMatrix(world);

		for (int j = 0; j < l_RenderableObject->GetNumSubmeshes(); ++j)
		{
			const CRenderableObject::SSubmesh& l_Submesh = l_RenderableObject->GetSubmesh(j);
			const CMaterial* l_Material = _MaterialManager->get(l_Submesh.material);
			// TODO no pintar el objeto, sino añadirlo a la lista l_SubmeshesWithBlend si tiene blend
			// l_Material->HasBlending();

			l_Material->SetShaderParameters(_Context);
			_Context->Draw(l_Submesh.vertices, l_Material->GetRasterizerState(), l_Material->GetDepthStencilState(), l_Material->GetBlendState());
		}
	}
	*/
	// TODO: Ordenar objetos según la distáncia a la cámara
	// NOTA: El quicksort es más rápido que el buble sort cuando la lista tiene más de ~100 objetos. NO OS MATÉIS SI NO HACE FALTA.
	//const Vect3f& l_CameraPosition = m_CurrentCamera.GetPosition();
	//for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	//{
	//    float l_DistanceSQ = l_SubmeshesWithBlend[i].position.SquaredLength(l_CameraPosition);
	//}

	// TODO: Pintar objetos translúcidos
	//for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	//{
	//
	//    // TODO render
	//
	//}
}
