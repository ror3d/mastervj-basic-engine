#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <d3d11.h>
#include "Graphics\Texture\Texture.h"
#include "Graphics\Effect\EffectTechnique.h"
#include <Base/Math/Color.h>

#define MAX_RENDER_TARGET_VIEW 4

class CRenderableVertexs;

class CContextManager
{
public:

	CColor m_BackgroundColor;

	CContextManager();
	~CContextManager();

	void destroy();

	void Resize(HWND hWnd, unsigned int Width, unsigned int Height);

	HRESULT CreateContext(HWND hWnd, int Width, int Height);
	HRESULT CreateBackBuffer(HWND hWnd, int Width, int Height);

	float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }
	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }

	void EnableAlphaBlendState();
	void DisableAlphaBlendState();
	void Clear(bool clear_DepthStencil, bool clear_RenderTarget);
	void Present();
	void SetRenderTargets(int NumViews, ID3D11RenderTargetView *const*RenderTargetViews, ID3D11DepthStencilView *DepthStencilView);
	void UnsetRenderTargets();
	void DrawScreenQuad(CEffectTechnique *EffectTechnique, CTexture
		*Texture, float x, float y, float Width, float Height, const CColor &Color);
	void DrawRelativeScreenQuad(CEffectTechnique *EffectTechnique, CTexture
		*Texture, float x, float y, float Width, float Height, const CColor &Color);

	ID3D11Device* GetDevice() const { return m_D3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_DepthStencilView; }
	IDXGISwapChain* GetSwapChain() const { return m_SwapChain;  }

private:

	ID3D11Device*			m_D3DDevice;
	ID3D11DeviceContext*	m_DeviceContext;
	ID3D11Debug*			m_D3DDebug;
	IDXGISwapChain*			m_SwapChain;
	ID3D11RenderTargetView*	m_RenderTargetView;
	ID3D11RenderTargetView* m_CurrentRenderTargetViews[MAX_RENDER_TARGET_VIEW];
	ID3D11Texture2D*		m_DepthStencil;
	ID3D11DepthStencilView*	m_DepthStencilView;
	ID3D11DepthStencilView* m_CurrentDepthStencilView;
	ID3D11BlendState*		m_AlphaBlendState;
	D3D11_VIEWPORT			m_Viewport;
	CRenderableVertexs*		m_DrawQuadRV;


	int m_Width, m_Height;
	int m_NumViews;
};

#endif
