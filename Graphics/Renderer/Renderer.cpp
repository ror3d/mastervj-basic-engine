#include "Renderer.h"
#include "Renderable/Renderable.h"
#include "Effect/EffectManager.h"
#include <algorithm>


CRenderer::CRenderer()
{}


CRenderer::~CRenderer()
{}

void CRenderer::AddRenderableToRender(const std::string& layer, IRenderable* mesh, const Mat44f& transf)
{
	DEBUG_ASSERT( mesh != nullptr );
	m_Layers[layer][mesh].push_back(transf);
}

void CRenderer::RenderLayer(const std::string& layer, CContextManager *context, bool m_layer_zsort)
{
	auto layerIt = m_Layers.find(layer);

	if (layerIt == m_Layers.end())
	{
		return;
	}


	//AQUI ordenar. Primero hay que hacer una funcion para ordenar por profundidad(Rendererlayerbydepth). Estructura igual al doble for que sigue pero en vez del renderer tiene que obtener la translacion
	//Una vez obtenidas las posiciones las restas con la de la camara (de sceneeffectparameters) y las pones en un vector para ordenarlas. std sort para ordenar un vector. Ha de ser un vector de parejas de 
	//transform y mesh con ese transform (al reves que ahora). Una vez hecho hacer render
	if (m_layer_zsort)
	{
		struct RenderTuple
		{
			IRenderable* mesh;
			float distance;
			Mat44f *transform;
		};

		std::vector<RenderTuple> m_orderedMeshes;


		for ( auto &const meshInstances : layerIt->second )
		{
			for ( auto & transf : meshInstances.second )
			{
				Vect4f camPos = CEffectManager::m_SceneParameters.m_CameraPosition;
				Vect4f objPos = transf.GetPos();
				m_orderedMeshes.push_back( RenderTuple { meshInstances.first, ( camPos - objPos ).SquaredLength(), &transf } );
			}
		}

		std::sort( m_orderedMeshes.begin(), m_orderedMeshes.end(),
				   []( RenderTuple &a, RenderTuple &b ) { return a.distance > b.distance; } );

		for (auto &const rt : m_orderedMeshes)
		{
			CEffectManager::m_SceneParameters.m_World = *rt.transform;
			rt.mesh->Render(context);
		}
	}
	else
	{
		for (auto &const meshInstances : layerIt->second)
		{
			for (auto &const transf : meshInstances.second)
			{
				CEffectManager::m_SceneParameters.m_World = transf;
				meshInstances.first->Render(context);
			}
		}
	}
}

void CRenderer::BeginRender()
{
}

void CRenderer::EndRender()
{
	m_Layers.clear();
}
