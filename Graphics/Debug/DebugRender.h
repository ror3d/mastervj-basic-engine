#pragma once

#include <d3d11.h>

#include <Base/Math/Math.h>

class CEffect;
class CRenderableVertexs;

class CDebugRender
{
	ID3D11VertexShader *m_VertexShader;
	ID3D11InputLayout *m_VertexLayout;
	ID3D11PixelShader *m_PixelShader;
	ID3D11Buffer *m_SceneConstantBufferV;
	ID3D11Buffer *m_SceneConstantBufferP;
	struct SceneConstantBuffer_t
	{
		Mat44f m_World;
		Mat44f m_View;
		Mat44f m_Projection;
	}
		m_SceneConstantBufferValues;
public:
	CDebugRender();
	~CDebugRender();
	void Init();

	void Render();

	bool m_RenderAxis;

	bool m_RenderLights;


private:

	CRenderableVertexs				*m_Axis;

};

