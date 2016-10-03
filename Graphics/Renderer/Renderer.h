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
	typedef std::map<std::string, Layer_t> LayerMap_t;

	LayerMap_t m_Layers;

public:
	CRenderer();
	~CRenderer();

	void BeginRender();

	void EndRender();

	void AddRenderableToRender(const std::string& layer, IRenderable* mesh, const Mat44f& transf);

	void RenderLayer(const std::string& layer, CContextManager *context, bool m_layer_zsort);

	void destroy() {}
};

