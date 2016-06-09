#pragma once

#include <map>
#include <vector>
#include <string>
#include <Base/Math/Math.h>

class CStaticMesh;
class CContextManager;

class CRenderer
{
	typedef std::map<CStaticMesh*, std::vector<Mat44f>> Layer_t;
	typedef std::map<std::string, Layer_t> LayerMap_t;

	LayerMap_t m_Layers;

public:
	CRenderer();
	~CRenderer();

	void BeginRender();

	void EndRender();

	void AddMeshToRender(const std::string& layer, CStaticMesh* mesh, const Mat44f& transf);

	void RenderLayer(const std::string& layer, CContextManager *context);
};

