#include "EffectShader.h"
#include "Engine/Engine.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "EffectParameters.h"

#include <sstream>

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
	m_Preprocessor = TreeNode.GetPszProperty("preprocessor", "", false);

	CreateShaderMacro();
}


CEffectShader::~CEffectShader()
{
	for ( auto buf : m_ConstantBuffers )
	{
		buf->Release();
	}
}

void SplitString(const std::string& str, char split, std::vector<std::string>& out)
{
	std::stringstream ss;
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		auto c = (*it);
		if (c == split)
		{
			out.push_back(ss.str());
			ss.str(std::string());
		}
		else
		{
			ss << c;
		}
	}

	if (ss.str().length() > 0)
	{
		out.push_back(ss.str());
	}
}

void CEffectShader::CreateShaderMacro()
{
	m_PreprocessorMacros.clear();
	m_ShaderMacros.clear();

	if (m_Preprocessor.empty())
	{
		return;
	}

	std::vector<std::string> l_PreprocessorItems;
	SplitString(m_Preprocessor, ';', l_PreprocessorItems);

	m_ShaderMacros.resize(l_PreprocessorItems.size() + 1);

	for (size_t i = 0; i<l_PreprocessorItems.size(); ++i)
	{
		std::vector<std::string> l_PreprocessorItem;
		SplitString(l_PreprocessorItems[i], '=', l_PreprocessorItem);
		if (l_PreprocessorItem.size() == 1)
		{
			m_PreprocessorMacros.push_back(l_PreprocessorItems[i]);
			m_PreprocessorMacros.push_back("1");
		}
		else if (l_PreprocessorItem.size() == 2)
		{
			m_PreprocessorMacros.push_back(l_PreprocessorItem[0]);
			m_PreprocessorMacros.push_back(l_PreprocessorItem[1]);
		}
		else
		{
			DEBUG_ASSERT(!"Error creating shader macro '%s', with wrong size on parameters");
			return;
		}
	}

	for (int i = 0; i<l_PreprocessorItems.size(); ++i)
	{
		m_ShaderMacros[i].Name = m_PreprocessorMacros[i * 2].c_str();
		m_ShaderMacros[i].Definition = m_PreprocessorMacros[(i * 2) + 1].c_str();
	}

	m_ShaderMacros[l_PreprocessorItems.size()].Name = NULL;
	m_ShaderMacros[l_PreprocessorItems.size()].Definition = NULL;

	l_PreprocessorItems.clear();
}

bool CEffectShader::LoadShader(const std::string &Filename, const std::string
							   &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(Filename.c_str(), m_ShaderMacros.data(), NULL,
							   EntryPoint.c_str(), ShaderModel.c_str(), dwShaderFlags, 0, NULL, BlobOut,
							   &pErrorBlob, NULL);
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

bool CEffectShader::CreateConstantBuffer(int IdBuffer, unsigned int BufferSize)
{
	ID3D11Buffer *l_ConstantBuffer;
	CContextManager &l_ContextManager = *CEngine::GetSingleton().getContextManager();
	ID3D11Device *l_Device = l_ContextManager.GetDevice();
	D3D11_BUFFER_DESC l_BufferDescription;
	ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
	l_BufferDescription.Usage = D3D11_USAGE_DEFAULT;
	l_BufferDescription.ByteWidth = BufferSize;
	if ((BufferSize % 16) != 0)
	{
		DEBUG_ASSERT(!"Constant Buffer '%d' with wrong size '%d' on shader '%s'.");
	}
	l_BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	l_BufferDescription.CPUAccessFlags = 0;
	if (FAILED(l_Device->CreateBuffer(&l_BufferDescription, NULL,
		&l_ConstantBuffer)))
	{
		DEBUG_ASSERT(!"Constant buffer '%d' couldn't created on shader '%s'.");
		m_ConstantBuffers.push_back(NULL);
		return false;
	}
	m_ConstantBuffers.push_back(l_ConstantBuffer);
	return true;
}


bool CEffectShader::CreateConstantBuffer()
{
	bool r0 = CreateConstantBuffer(0, sizeof(CSceneEffectParameters));
	DEBUG_ASSERT(r0);
	bool r1 = CreateConstantBuffer(1, sizeof(CLightEffectParameters));
	DEBUG_ASSERT(r1);
	bool r2 = CreateConstantBuffer(2, sizeof(CAnimatedModelEffectParameters));
	DEBUG_ASSERT(r2);
	bool r3 = CreateConstantBuffer(3, sizeof(CMaterialEffectParameters));
	DEBUG_ASSERT(r3);
	return ( r0 && r1 && r2 && r3 );
}


bool CEffectShader::Reload()
{
	return true;
}
