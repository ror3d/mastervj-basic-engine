#ifndef RENDERABLE_VERTEXS_H
#define RENDERABLE_VERTEXS_H

#include "Graphics/Context/ContextManager.h"

#include "Graphics/Effect/EffectTechnique.h"
#include "Graphics/Effect/EffectVertexShader.h"
#include "Graphics/Effect/EffectPixelShader.h"
#include "Graphics/Effect/EffectGeometryShader.h"

#include <Utils/Utils.h>

#include <Core/Engine/Engine.h>

#include <D3D11.h>

class CContextManager;
class CEffect;

class CRenderableVertexs
{
public:
	virtual bool UpdateVertices(ID3D11DeviceContext* context, void *Vtxs, unsigned int vtxCount)
	{
		DEBUG_ASSERT(!"This method mustn't be called");
		return false;
	}
	virtual bool Render(CContextManager *ContextManager, CEffectTechnique *Effect)
	{
		DEBUG_ASSERT(!"This method mustn't be called");
		return false;
	}
	virtual bool Render(ID3D11DeviceContext* context, ID3D11InputLayout* inputLayout, ID3D11VertexShader* vshader, ID3D11PixelShader* pshader, ID3D11GeometryShader* gshader = NULL)
	{
		DEBUG_ASSERT(!"This method mustn't be called");
		return false;
	}
	virtual bool RenderIndexed(CContextManager *ContextManager, CEffectTechnique *EffectManager, unsigned int IndexCount = -1, unsigned int StartIndexLocation = 0, unsigned int BaseVertexLocation = 0)
	{
		DEBUG_ASSERT(!"This method mustn't be called");
		return false;
	}
	virtual bool RenderIndexed(ID3D11DeviceContext* context, ID3D11InputLayout* inputLayout, ID3D11VertexShader* vshader, ID3D11PixelShader* pshader, unsigned int IndexCount = -1, unsigned int StartIndexLocation = 0, unsigned int BaseVertexLocation = 0)
	{
		DEBUG_ASSERT(!"This method mustn't be called");
		return false;
	}
};

template<class T, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology>
class CTemplatedRenderableVertexs : public CRenderableVertexs
{
private:
	ID3D11Buffer *m_VertexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	unsigned int m_VertexsCount;
	unsigned int m_PrimitiveCount;
public:
	CTemplatedRenderableVertexs( void *Vtxs, unsigned int VtxsCount, unsigned int PrimitiveCount, bool Dynamic = false )
		: m_VertexsCount(VtxsCount)
		, m_PrimitiveTopology(PrimitiveTopology)
		, m_PrimitiveCount(PrimitiveCount)
	{
		D3D11_BUFFER_DESC l_BufferDescription;
		ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
		l_BufferDescription.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		l_BufferDescription.ByteWidth = sizeof(T)*m_VertexsCount;
		l_BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		l_BufferDescription.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = Vtxs;
		ID3D11Device *d3dDevice = CEngine::GetSingleton().getContextManager()->GetDevice();
		HRESULT l_HR = d3dDevice->CreateBuffer(&l_BufferDescription, &InitData, &m_VertexBuffer);
		if (FAILED(l_HR))
			return;
	}
	virtual ~CTemplatedRenderableVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
	}
	bool Render(CContextManager *ContextManager, CEffectTechnique *Effect)
	{
		if (Effect->GetPixelShader() == NULL || Effect->GetVertexShader() == NULL)
			return false;
		ID3D11DeviceContext *l_DeviceContext = ContextManager->GetDeviceContext();
		auto geom = Effect->GetGeometryShader();
		return Render(l_DeviceContext, Effect->GetVertexShader()->GetVertexLayout(), Effect->GetVertexShader()->GetVertexShader(), Effect->GetPixelShader()->GetPixelShader(), (geom ? geom->GetGeometryShader() : NULL));
	}
	bool Render(ID3D11DeviceContext* context, ID3D11InputLayout* inputLayout, ID3D11VertexShader* vshader, ID3D11PixelShader* pshader, ID3D11GeometryShader* gshader = NULL)
	{
		UINT stride = sizeof(T);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(m_PrimitiveTopology);
		context->IASetInputLayout(inputLayout);
		context->VSSetShader(vshader, NULL, 0);
		if ( gshader )
		{
			context->GSSetShader( gshader, NULL, 0 );
		}
		context->PSSetShader(pshader, NULL, 0);
		context->Draw(m_VertexsCount, 0);
		if ( gshader )
		{
			context->GSSetShader( NULL, NULL, 0 );
		}
		return true;
	}

	bool UpdateVertices( ID3D11DeviceContext* context, void *Vtxs, unsigned int vtxCount )
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory( &mappedResource, sizeof( mappedResource ) );

		HRESULT hr = context->Map( m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		if ( FAILED( hr ) )
			return false;

		memcpy( mappedResource.pData, Vtxs, sizeof( T ) * vtxCount );

		context->Unmap( m_VertexBuffer, 0 );

		m_VertexsCount = vtxCount;

		return true;
	}
};



template<class T, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, DXGI_FORMAT IndexType>
class CTemplatedRenderableIndexedVertexs : public CRenderableVertexs
{
protected:
	ID3D11Buffer *m_VertexBuffer;
	ID3D11Buffer *m_IndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	DXGI_FORMAT m_IndexType;
	unsigned int m_VertexsCount;
	unsigned int m_IndexsCount;
public:
	CTemplatedRenderableIndexedVertexs(void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount)
		: m_VertexsCount(VtxsCount)
		, m_IndexsCount(IndexsCount)
		, m_PrimitiveTopology(PrimitiveTopology)
		, m_VertexBuffer(0)
		, m_IndexBuffer(0)
		, m_IndexType(IndexType)
	{
		D3D11_BUFFER_DESC l_VertexBufferDesc;
		ZeroMemory(&l_VertexBufferDesc, sizeof(l_VertexBufferDesc));
		l_VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		l_VertexBufferDesc.ByteWidth = sizeof(T)*m_VertexsCount;
		l_VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		l_VertexBufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = Vtxs;
		ID3D11Device *d3dDevice = CEngine::GetSingleton().getContextManager()->GetDevice();
		HRESULT hr = d3dDevice->CreateBuffer(&l_VertexBufferDesc, &InitData, &m_VertexBuffer);
		if (FAILED(hr))
			return;
		D3D11_BUFFER_DESC l_IndexBuffer;
		ZeroMemory(&l_IndexBuffer, sizeof(l_IndexBuffer));
		l_IndexBuffer.Usage = D3D11_USAGE_DEFAULT;
		l_IndexBuffer.ByteWidth = (m_IndexType == DXGI_FORMAT_R16_UINT ? sizeof(WORD) : sizeof(unsigned int))*m_IndexsCount;
		l_IndexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
		l_IndexBuffer.CPUAccessFlags = 0;
		InitData.pSysMem = Indices;
		hr = d3dDevice->CreateBuffer(&l_IndexBuffer, &InitData, &m_IndexBuffer);
		if (FAILED(hr))
			return;
	}

	virtual ~CTemplatedRenderableIndexedVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
		CHECKED_RELEASE(m_IndexBuffer);
	}

	bool RenderIndexed(CContextManager *ContextManager, CEffectTechnique *Effect, unsigned int IndexCount = -1, unsigned int StartIndexLocation = 0, unsigned int BaseVertexLocation = 0)
	{
		if (Effect == NULL)
			return false;
		if (Effect->GetPixelShader() == NULL || Effect->GetVertexShader() == NULL)
			return false;
		ID3D11DeviceContext *l_DeviceContext = ContextManager->GetDeviceContext();
		return RenderIndexed(l_DeviceContext, Effect->GetVertexShader()->GetVertexLayout(), Effect->GetVertexShader()->GetVertexShader(), Effect->GetPixelShader()->GetPixelShader(), IndexCount, StartIndexLocation, BaseVertexLocation);
	}
	inline bool RenderIndexed(ID3D11DeviceContext* context, ID3D11InputLayout* inputLayout, ID3D11VertexShader* vshader, ID3D11PixelShader* pshader, unsigned int IndexCount = -1, unsigned int StartIndexLocation = 0, unsigned int BaseVertexLocation = 0)
	{
		UINT stride = sizeof(T);
		UINT offset = 0;
		context->IASetIndexBuffer(m_IndexBuffer, m_IndexType, 0);
		context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(m_PrimitiveTopology);
		context->IASetInputLayout(inputLayout);
		context->VSSetShader(vshader, NULL, 0);
		context->PSSetShader(pshader, NULL, 0);
		context->DrawIndexed(IndexCount == -1 ? m_IndexsCount : IndexCount, StartIndexLocation, BaseVertexLocation);
		return true;

	}
};



template<class T>
using CPointsListRenderableVertexs = CTemplatedRenderableVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST>;

template<class T>
using CLinesListRenderableVertexs = CTemplatedRenderableVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_LINELIST>;
template<class T>
using CTrianglesListRenderableVertexs = CTemplatedRenderableVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST>;
template<class T>
using CTrianglesStripRenderableVertexs = CTemplatedRenderableVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP>;
template<class T>
using CLinesStripRenderableVertexs = CTemplatedRenderableVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP>;


template<class T>
using CKGTriangleListRenderableIndexed16Vertexs	= CTemplatedRenderableIndexedVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R16_UINT>;
template<class T>
using CKGTriangleListRenderableIndexed32Vertexs	= CTemplatedRenderableIndexedVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R32_UINT>;
template<class T>
using CKGTriangleStripRenderableIndexed16Vertexs= CTemplatedRenderableIndexedVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R16_UINT>;
template<class T>
using CKGTriangleStripRenderableIndexed32Vertexs= CTemplatedRenderableIndexedVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R32_UINT>;
template<class T>
using CLinesListRenderableIndexed16Vertexs		= CTemplatedRenderableIndexedVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, DXGI_FORMAT_R16_UINT>;
template<class T>
using CLinesListRenderableIndexed32Vertexs		= CTemplatedRenderableIndexedVertexs<T, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, DXGI_FORMAT_R32_UINT>;


#endif
