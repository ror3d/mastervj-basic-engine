#include "DebugRender.h"

#include "Mesh/VertexTypes.h"

#include "Mesh/RenderableVertexs.h"

#include <Graphics/Effect/EffectManager.h>

#include <D3DX11.h>
#include <D3Dcompiler.h>

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#endif

CDebugRender::CDebugRender()
	: m_RenderAxis(true)
	, m_VertexShader(nullptr)
	, m_VertexLayout(nullptr)
	, m_PixelShader(nullptr)
	, m_SceneConstantBufferV(nullptr)
	, m_SceneConstantBufferP(nullptr)
{
}

CDebugRender::~CDebugRender()
{
	delete m_Axis;
	m_Axis = NULL;

	if (m_VertexLayout)
		m_VertexLayout->Release();
	if (m_VertexShader)
		m_VertexShader->Release();
	if (m_PixelShader)
		m_PixelShader->Release();
	if (m_SceneConstantBufferV)
		m_SceneConstantBufferV->Release();
	if (m_SceneConstantBufferP)
		m_SceneConstantBufferP->Release();
}

void CDebugRender::Init()
{
	{
		//AXIS
		MV_POSITION_COLOR_VERTEX l_AxisVtxs[6] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vect3f(1.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vect3f(0.0f, 1.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) }
		};

		m_Axis = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_AxisVtxs, 6, 3);
	}

	{
		HRESULT hr = S_OK;
		ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
		ID3DBlob* pErrorBlob;
		std::string shader =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 m_World;"
"	float4x4 m_View;"
"	float4x4 m_Projection;"
"}"

"struct VS_INPUT"
"{"
"	float3 Pos : POSITION;"
"	float4 Color : COLOR;"
"};"
"struct PS_INPUT"
"{"
"	float4 Pos : SV_POSITION;"
"	float4 Color : COLOR;"
"};"
"PS_INPUT VS(VS_INPUT l_in)"
"{"
"	PS_INPUT l_out = (PS_INPUT)0;"
"	float4 l_Position = float4(l_in.Pos.xyz, 1.0);"
"	l_Position = mul(l_Position, m_World);"
"	l_Position = mul(l_Position, m_View);"
"	l_out.Pos = mul(l_Position, m_Projection);"

"	l_out.Color = l_in.Color;"
"	return l_out;"
"}"

"float4 PS(PS_INPUT l_in) : SV_Target"
"{"
"	return l_in.Color;"
"}\n";
		{
			ID3DBlob *l_VSBlob = NULL;
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
			dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
			hr = D3DX11CompileFromMemory(shader.data(), shader.size(), "debug_shader", NULL, NULL, "VS", "vs_4_0", dwShaderFlags, 0, NULL, &l_VSBlob, &pErrorBlob, NULL);
			if (FAILED(hr))
			{
				if (pErrorBlob != NULL)
					OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
				if (pErrorBlob)
					pErrorBlob->Release();
				return;
			}
			if (pErrorBlob)
				pErrorBlob->Release();
			hr = l_Device->CreateVertexShader(l_VSBlob->GetBufferPointer(),
										 l_VSBlob->GetBufferSize(), NULL,
										 &m_VertexShader);
			if (FAILED(hr))
			{
				l_VSBlob->Release();
				return;
			}
			bool l_Loaded = MV_POSITION_COLOR_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
			l_VSBlob->Release();
			if (!l_Loaded)
			{
				return;
			}
		}
		{
			ID3DBlob *l_PSBlob = NULL;
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
			dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
			hr = D3DX11CompileFromMemory(shader.data(), shader.size(), "debug_shader", NULL, NULL, "PS", "ps_4_0", dwShaderFlags, 0, NULL, &l_PSBlob, &pErrorBlob, NULL);
			if (FAILED(hr))
			{
				if (pErrorBlob != NULL)
					OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
				if (pErrorBlob)
					pErrorBlob->Release();
				m_VertexShader->Release();
				m_VertexShader = 0;
				m_VertexLayout->Release();
				m_VertexLayout = 0;
				return;
			}
			if (pErrorBlob)
				pErrorBlob->Release();
			hr = l_Device->CreatePixelShader(l_PSBlob->GetBufferPointer(),
										 l_PSBlob->GetBufferSize(), NULL,
										 &m_PixelShader);
			l_PSBlob->Release();
			if (FAILED(hr))
			{
				m_VertexShader->Release();
				m_VertexShader = 0;
				m_VertexLayout->Release();
				m_VertexLayout = 0;
				return;
			}
		}
		{
			D3D11_BUFFER_DESC l_BufferDescription;
			ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
			l_BufferDescription.Usage = D3D11_USAGE_DEFAULT;
			l_BufferDescription.ByteWidth = sizeof(SceneConstantBuffer_t);
			l_BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			l_BufferDescription.CPUAccessFlags = 0;
			if (FAILED(l_Device->CreateBuffer(&l_BufferDescription, NULL,
				&m_SceneConstantBufferV)))
			{
				DEBUG_ASSERT(!"Constant buffer '%d' couldn't created on shader '%s'.");
				m_VertexShader->Release();
				m_VertexShader = 0;
				m_VertexLayout->Release();
				m_VertexLayout = 0;
			}
			if (FAILED(l_Device->CreateBuffer(&l_BufferDescription, NULL,
				&m_SceneConstantBufferP)))
			{
				DEBUG_ASSERT(!"Constant buffer '%d' couldn't created on shader '%s'.");
				m_VertexShader->Release();
				m_VertexShader = 0;
				m_VertexLayout->Release();
				m_VertexLayout = 0;
				m_SceneConstantBufferV->Release();
				m_SceneConstantBufferV = nullptr;
			}
		}
	}
}

void CDebugRender::Render()
{
	if (!m_SceneConstantBufferV) return;

	auto cm = CEngine::GetSingleton().getContextManager();
	m_SceneConstantBufferValues.m_Projection = CEffectManager::m_SceneParameters.m_Projection;
	m_SceneConstantBufferValues.m_View = CEffectManager::m_SceneParameters.m_View;
	m_SceneConstantBufferValues.m_World.SetIdentity();

	ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().getContextManager()->GetDeviceContext();
	l_DeviceContext->
		UpdateSubresource(m_SceneConstantBufferV, 0, NULL,
									   &m_SceneConstantBufferValues, 0, 0);
	l_DeviceContext->
		UpdateSubresource(m_SceneConstantBufferP, 0, NULL,
									   &m_SceneConstantBufferValues, 0, 0);
	l_DeviceContext->VSSetConstantBuffers(0, 1, &m_SceneConstantBufferV);
	l_DeviceContext->PSSetConstantBuffers(0, 1, &m_SceneConstantBufferP);

	m_Axis->Render(l_DeviceContext, m_VertexLayout, m_VertexShader, m_PixelShader);

}
