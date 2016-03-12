#include "DebugRender.h"

#include "VertexTypes.h"

#include "Renderable/RenderableVertexs.h"

#include <D3DX11.h>
#include <D3Dcompiler.h>

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3d11.lib")
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
	delete m_ClassicBlendTriangle;
	delete m_SimpleTriangle;
	delete m_PremultBlendTriangle;
	delete m_SimpleGrid;
	//delete m_SimpleCube;
	delete m_Axis;
	delete m_LongAxis;
	delete m_DrawQuadRV;
	m_ClassicBlendTriangle = NULL;
	m_SimpleTriangle = NULL;
	m_PremultBlendTriangle = NULL;
	m_SimpleGrid = NULL;
	m_SimpleCube = NULL;
	m_Axis = NULL;
	m_LongAxis = NULL;
	m_DrawQuadRV = NULL;

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
		m_AxisBSRadi = 1;
		m_AxisBBMin = Vect3f(0, 0, 0);
		m_AxisBBMax = Vect3f(1, 1, 1);
	}
	{
		MV_POSITION_COLOR_VERTEX l_LongAxisVtxs[12] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(1.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 1.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-1.0f, 0.0f, 0.0f), CColor(0.0f, 1.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, -1.0f, 0.0f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 0.0f, 1.0f) }
		};

		m_LongAxis = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_LongAxisVtxs, 12, 6);
		m_LongAxisBSRadi = 1;
		m_LongAxisBBMin = Vect3f(-1, -1, -1);
		m_LongAxisBBMax = Vect3f(1, 1, 1);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
		};

		m_SimpleTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1);
		m_SimpleTriangleBSRadi = 0.86603f;
		m_SimpleTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_SimpleTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(1.0f, 0.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 1.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 1.0f, 0.5f) },
		};

		m_ClassicBlendTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1);
		m_ClassicBlendTriangleBSRadi = 0.86603f;
		m_ClassicBlendTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_ClassicBlendTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(0.5f, 0.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 0.5f, 0.0f, 0.5f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 0.5f, 0.5f) },
		};

		m_PremultBlendTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1);
		m_PremultBlendTriangleBSRadi = 0.86603f;
		m_PremultBlendTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_PremultBlendTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		// Simple Grid
		MV_POSITION_COLOR_VERTEX l_SimpleGridVtxs[] =
		{
			{ Vect3f(+1.0f, 0.0f, +1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },  // 0
			{ Vect3f(+1.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 0.0f, 1.0f) },  // 1
			{ Vect3f(-1.0f, 0.0f, -1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },  // 2
			{ Vect3f(-1.0f, 0.0f, +1.0f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },  // 3
			{ Vect3f(+1.0f, 0.0f, +0.0f), CColor(1.0f, 1.0f, 0.5f, 1.0f) },  // 4
			{ Vect3f(-1.0f, 0.0f, +0.0f), CColor(1.0f, 0.0f, 0.5f, 1.0f) },  // 5
			{ Vect3f(+0.0f, 0.0f, +1.0f), CColor(1.0f, 0.5f, 1.0f, 1.0f) },  // 6
			{ Vect3f(+0.0f, 0.0f, -1.0f), CColor(1.0f, 0.5f, 0.0f, 1.0f) },  // 7
		};

		uint16_t l_SimpleGridIdxs[] = { 0, 1, 0, 3, 2, 1, 2, 3, 4, 5, 6, 7 };

		m_SimpleGrid = new CLinesListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleGridVtxs, 8, l_SimpleGridIdxs, 12);
		m_SimpleGridBSRadi = 1.414236f;
		m_SimpleGridBBMin = Vect3f(-1.f, 0.f, -1.f);
		m_SimpleGridBBMax = Vect3f(1.f, 0.f, 1.f);
	}
	{
		// Simple Cube
		MV_POSITION_COLOR_VERTEX l_SimpleCubeVtxs[] =
		{
			{ Vect3f(+0.5f, 0.0f, +0.5f), CColor(0.0f, 1.0f, 1.0f, 1.0f) },   // 0
			{ Vect3f(+0.5f, 0.0f, -0.5f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },  // 1
			{ Vect3f(-0.5f, 0.0f, -0.5f), CColor(0.0f, 0.0f, 0.0f, 1.0f) }, // 2
			{ Vect3f(-0.5f, 0.0f, +0.5f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },  // 3
			{ Vect3f(+0.5f, 1.0f, +0.5f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },   // 4
			{ Vect3f(+0.5f, 1.0f, -0.5f), CColor(1.0f, 1.0f, 0.0f, 1.0f) },  // 5
			{ Vect3f(-0.5f, 1.0f, -0.5f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },   // 6
			{ Vect3f(-0.5f, 1.0f, +0.5f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },  // 7
		};


		//uint16_t l_SimpleCubeIdxs[] = {
		//0, 1, 0, 3, 2, 1, 2, 3,
		//4, 5, 4, 7, 6, 5, 6, 7,
		//0, 4, 1, 5, 2, 6, 3, 7 };

		uint16_t l_SimpleCubeIdxs[] = {
			0, 2, 1, 3, 2, 0, // bot
			0, 4, 3, 3, 4, 7, // front
			0, 1, 4, 4, 1, 5,
			3, 7, 2, 2, 7, 6,
			1, 2, 5, 2, 6, 5,
			4, 5, 6, 6, 7, 4,
		};

		//m_SimpleCube = new CLinesListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(_Device, l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs, 24);

		//m_SimpleCube = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs, 36);
		m_SimpleCubeBSRadi = 1.224745f;
		m_SimpleCubeBBMin = Vect3f(-.5f, 0.f, -.5f);
		m_SimpleCubeBBMax = Vect3f(.5f, 1.f, .5f);
	}
	//QUAD
	{		
		MV_POSITION4_COLOR_TEXTURE_VERTEX l_ScreenVertexsQuad[4] =
		{
			{ Vect4f(-1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(0.0f, 0.0f) },
			{ Vect4f(-1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(0.0f, 1.0f) },
			{ Vect4f(1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(1.0f, 0.0f) },
			{ Vect4f(1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(1.0f, 1.0f) }
		};
		m_DrawQuadRV = new CTrianglesStripRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_ScreenVertexsQuad, 4, 2);
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
"	l_Position = mul(l_Position.xyz, m_World);"
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
	m_SceneConstantBufferValues.m_World.Translate(Vect3f(0.5, 0, 0.5));

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
