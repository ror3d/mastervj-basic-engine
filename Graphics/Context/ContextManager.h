#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <d3d11.h>

class CRenderableVertexs;
class CColor;

class CContextManager
{
public:
	CContextManager();
	~CContextManager();

	void destroy();

	void Resize(HWND hWnd, unsigned int Width, unsigned int Height);

	HRESULT CreateContext(HWND hWnd, int Width, int Height);
	HRESULT CreateBackBuffer(HWND hWnd, int Width, int Height);

	float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }

	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }

	void BeginRender();
	void BeginRender(const CColor &backgroundColor);
	void EndRender();

	ID3D11Device* GetDevice() const { return m_D3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }

	IDXGISwapChain* GetSwapChain() const { return m_SwapChain;  }

private:

	ID3D11Device*			m_D3DDevice;
	ID3D11DeviceContext*	m_DeviceContext;
	ID3D11Debug*			m_D3DDebug;
	IDXGISwapChain*			m_SwapChain;
	ID3D11RenderTargetView*	m_RenderTargetView;
	ID3D11Texture2D*		m_DepthStencil;
	ID3D11DepthStencilView*	m_DepthStencilView;

	int m_Width, m_Height;
};

#endif
