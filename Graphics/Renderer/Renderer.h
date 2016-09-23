#pragma once

#include <map>
#include <vector>
#include <string>
#include <Base/Math/Math.h>

class IRenderable;
class CContextManager;

class CRenderer
{
	typedef std::map<IRenderable*, std::vector<Mat44f>> Layer_t;
	//typedef std::multimap<std::vector<Mat44f>, IRenderable*> Layer_t_inverse;
	typedef std::map<std::string, Layer_t> LayerMap_t;

	LayerMap_t m_Layers;

	struct LayerVector{
		float distance;
		Matrix44<float> transform;
		IRenderable* mesh;
	};

public:
	CRenderer();
	~CRenderer();

	void BeginRender();

	void EndRender();

	void AddRenderableToRender(const std::string& layer, IRenderable* mesh, const Mat44f& transf);

	void RenderLayer(const std::string& layer, CContextManager *context, bool m_layer_zsort);

	void CRenderer::RenderLayerByDepth(LayerMap_t::iterator layerIt, CContextManager *context);

	float CRenderer::CalcDistance(Vect3f dist1, Vect3f dist2);

	static bool CRenderer::CompareDistance(const LayerVector &a, const LayerVector &b);

	void destroy() {}
};

