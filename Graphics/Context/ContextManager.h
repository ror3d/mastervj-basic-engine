#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <d3d11.h>

class CRenderableVertexs;
class CColor;

class CContextManager
{
public:

	enum ERasterizerState
	{
		RS_WIREFRAME,
		RS_SOLID,
		// TODO crear un modo que haga culling de la cara frontal y otra de la cara trasera

		RS_COUNT
	};

	enum EDepthStencilState
	{
		DSS_DEPTH_ON,
		DSS_OFF,
		// TODO: Crear un modo que haga el depth test, pero no escriba su posición

		DSS_COUNT
	};

	enum EBlendState
	{
		BLEND_SOLID,
		BLEND_CLASSIC,
		BLEND_PREMULT,

		BLEND_COUNT
	};

public:
	CContextManager();
	~CContextManager();

	void Dispose();

	void Resize(HWND hWnd, unsigned int Width, unsigned int Height);

	HRESULT CreateContext(HWND hWnd, int Width, int Height);
	HRESULT CreateBackBuffer(HWND hWnd, int Width, int Height);
	void InitStates();

	float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }

	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }

	void BeginRender();
	void BeginRender(const CColor &backgroundColor);
	void EndRender();

	ID3D11Device* GetDevice() const { return m_D3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }

private:

	void InitRasterizerStates();
	void InitDepthStencilStates();
	void InitBlendStates();

	ID3D11Device*			m_D3DDevice;
	ID3D11DeviceContext*	m_DeviceContext;
	ID3D11Debug*			m_D3DDebug;
	IDXGISwapChain*			m_SwapChain;
	ID3D11RenderTargetView*	m_RenderTargetView;
	ID3D11Texture2D*		m_DepthStencil;
	ID3D11DepthStencilView*	m_DepthStencilView;

	int m_Width, m_Height;

	ID3D11RasterizerState*	m_RasterizerSates[RS_COUNT];
	ID3D11DepthStencilState * m_DepthStencilStates[DSS_COUNT];
	ID3D11BlendState* m_BlendStates[BLEND_COUNT];
};

#endif
