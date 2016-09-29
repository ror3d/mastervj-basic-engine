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

float CRenderer::CalcDistance(Vect3f dist1, Vect3f dist2)
{
	float diffx = dist2.x - dist1.x;
	float diffy = dist2.y - dist1.y;
	float diffz = dist2.z - dist1.z;

	return sqrt((float)(diffx * diffx + diffy * diffy + diffz * diffz));
}

bool CRenderer::CompareDistance(const LayerVector &a, const LayerVector &b)
{
	return a.distance > b.distance;
}

void CRenderer::RenderLayerByDepth(LayerMap_t::iterator layerIt, CContextManager *context)
{
	Vect3f Cameraposition;
	Vect3f ObjPosition;
	std::vector<LayerVector> DistLayerVector;
	LayerVector Layer;

	for (auto &const meshInstances : layerIt->second)
	{
		for (auto &const transf : meshInstances.second)
		{
			Cameraposition = (CEffectManager::m_SceneParameters.m_CameraPosition.x, CEffectManager::m_SceneParameters.m_CameraPosition.y, CEffectManager::m_SceneParameters.m_CameraPosition.z);
			ObjPosition = Vect3f(transf.m30, transf.m31, transf.m32);
			Layer.distance = CalcDistance(Cameraposition, ObjPosition);
			Layer.transform = transf;
			Layer.mesh = meshInstances.first;
			DistLayerVector.push_back(Layer);
		}
	}
	std::sort(DistLayerVector.begin(), DistLayerVector.end(), CRenderer::CompareDistance);

	for (auto &const it = DistLayerVector.begin(); it != DistLayerVector.end(); ++it) {
		CEffectManager::m_SceneParameters.m_World = it->transform;
		it->mesh->Render(context);
	}
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
		RenderLayerByDepth(layerIt, context);
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
