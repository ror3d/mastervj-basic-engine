#include "EffectShader.h"
#include "Effect/EffectParameters.h"
#include "Engine/Engine.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3d11.lib")
#endif

CEffectShader::CEffectShader(const CXMLTreeNode &TreeNode)
	: CNamed(TreeNode)
{
	m_Filename = TreeNode.GetPszProperty("file");
	m_ShaderModel = TreeNode.GetPszProperty("shader_model");
	m_EntryPoint = TreeNode.GetPszProperty("entry_point");
}


CEffectShader::~CEffectShader()
{
}


bool CEffectShader::LoadShader(const std::string &Filename, const std::string &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif
	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(Filename.c_str(), NULL, NULL, EntryPoint.c_str(), ShaderModel.c_str(), dwShaderFlags, 0, NULL, BlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob)
			pErrorBlob->Release();
		return false;
	}
	if (pErrorBlob)
		pErrorBlob->Release();
	return true;
}


bool CEffectShader::CreateConstantBuffer()
{
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	D3D11_BUFFER_DESC l_BufferDescription;
	ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
	l_BufferDescription.Usage = D3D11_USAGE_DEFAULT;
	l_BufferDescription.ByteWidth = sizeof(CEffectParameters);
	l_BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	l_BufferDescription.CPUAccessFlags = 0;
	if (FAILED(l_Device->CreateBuffer(&l_BufferDescription, NULL, &m_ConstantBuffer)))
		return false;
	return true;
}


bool CEffectShader::Reload()
{
	return true;
}
