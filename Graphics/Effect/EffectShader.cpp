#include "EffectShader.h"
#include "Engine/Engine.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "EffectParameters.h"
#include <Graphics/Context/ContextManager.h>

#include <sstream>

#ifdef _DEBUG
	#pragma comment(lib, "d3dx11d.lib")
#else
	#pragma comment(lib, "d3dx11.lib")
#endif

#pragma comment(lib, "D3DCompiler.lib")

CEffectShader::CEffectShader(const CXMLTreeNode& TreeNode)
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

void SplitString(const std::string& str, char split,
                 std::vector<std::string>& out)
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

	for (size_t i = 0; i < l_PreprocessorItems.size(); ++i)
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
			DEBUG_ASSERT(
			    !"Error creating shader macro '%s', with wrong size on parameters");
			return;
		}
	}

	for (int i = 0; i < l_PreprocessorItems.size(); ++i)
	{
		m_ShaderMacros[i].Name = m_PreprocessorMacros[i * 2].c_str();
		m_ShaderMacros[i].Definition = m_PreprocessorMacros[(i * 2) + 1].c_str();
	}

	m_ShaderMacros[l_PreprocessorItems.size()].Name = NULL;
	m_ShaderMacros[l_PreprocessorItems.size()].Definition = NULL;

	l_PreprocessorItems.clear();
}

bool CEffectShader::LoadShader(const std::string& Filename, const std::string
                               &EntryPoint, const std::string& ShaderModel, ID3DBlob** BlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;

	CreateDirectory(".\\Cache", NULL);
	CreateDirectory(".\\Cache\\Shaders", NULL);
	std::string cachedFileName = "./Cache/Shaders/" + getName() + ".ofx";

	auto LoadCachedFile = [&](ID3DBlob** BlobOut)
	{
		FILE* l_cachedFile = NULL;
		fopen_s(&l_cachedFile, cachedFileName.c_str(), "rb");

		if (l_cachedFile == NULL)
		{
			return false;
		}

		size_t cFNameL;
		fread(&cFNameL, sizeof(cFNameL), 1, l_cachedFile);
		std::string cFname(cFNameL, ' ');
		fread(&(cFname[0]), 1, cFNameL, l_cachedFile);

		if (cFname != Filename)
		{
			fclose(l_cachedFile);
			return false;
		}

		size_t cEntryPL;
		fread(&cEntryPL, sizeof(cEntryPL), 1, l_cachedFile);
		std::string cEntryP(cEntryPL, ' ');
		fread(&(cEntryP[0]), 1, cEntryPL, l_cachedFile);

		if (cEntryP != EntryPoint)
		{
			fclose(l_cachedFile);
			return false;
		}

		size_t cShaderML;
		fread(&cShaderML, sizeof(cShaderML), 1, l_cachedFile);
		std::string cShaderM(cShaderML, ' ');
		fread(&(cShaderM[0]), 1, cShaderML, l_cachedFile);

		if (cShaderM != ShaderModel)
		{
			fclose(l_cachedFile);
			return false;
		}

		HANDLE hOriginalFile = CreateFile(Filename.c_str(), GENERIC_READ,
		                                  FILE_SHARE_READ, NULL,
		                                  OPEN_EXISTING, 0, NULL);
		FILETIME origWriteTime;
		GetFileTime(hOriginalFile, NULL, NULL, &origWriteTime);
		CloseHandle(hOriginalFile);

		HANDLE hCachedFile = CreateFile(cachedFileName.c_str(), GENERIC_READ,
		                                FILE_SHARE_READ, NULL,
		                                OPEN_EXISTING, 0, NULL);
		FILETIME cachedWriteTime;
		GetFileTime(hCachedFile, NULL, NULL, &cachedWriteTime);
		CloseHandle(hCachedFile);

		ULARGE_INTEGER origTimeLL;
		ULARGE_INTEGER cachedTimeLL;

		origTimeLL.HighPart = origWriteTime.dwHighDateTime;
		origTimeLL.LowPart = origWriteTime.dwLowDateTime;
		cachedTimeLL.HighPart = cachedWriteTime.dwHighDateTime;
		cachedTimeLL.LowPart = cachedWriteTime.dwLowDateTime;

		if (origTimeLL.QuadPart > cachedTimeLL.QuadPart)
		{
			fclose(l_cachedFile);
			return false;
		}

		size_t cShaderBCL;
		fread(&cShaderBCL, sizeof(cShaderBCL), 1, l_cachedFile);

		uint8* data = new uint8[cShaderBCL];
		fread(data, 1, cShaderBCL, l_cachedFile);


		D3DCreateBlob(cShaderBCL, BlobOut);

		memcpy((*BlobOut)->GetBufferPointer(), data, cShaderBCL);

		delete[] data;

		fclose(l_cachedFile);
		return true;
	};

	auto SaveFileToCache = [&](ID3DBlob * blob)
	{
		FILE* l_cachedFile = NULL;
		fopen_s(&l_cachedFile, cachedFileName.c_str(), "wb");

		if (l_cachedFile == NULL)
		{
			return;
		}

		size_t cFNameL = Filename.length();
		fwrite(&cFNameL, sizeof(size_t), 1, l_cachedFile);
		fwrite(Filename.c_str(), 1, cFNameL, l_cachedFile);

		size_t cEntryPL = EntryPoint.length();
		fwrite(&cEntryPL, sizeof(size_t), 1, l_cachedFile);
		fwrite(EntryPoint.c_str(), 1, cEntryPL, l_cachedFile);

		size_t cShaderML = ShaderModel.length();
		fwrite(&cShaderML, sizeof(size_t), 1, l_cachedFile);
		fwrite(ShaderModel.c_str(), 1, cShaderML, l_cachedFile);

		size_t cShaderBCL = blob->GetBufferSize();
		fwrite(&cShaderBCL, sizeof(size_t), 1, l_cachedFile);
		fwrite(blob->GetBufferPointer(), 1, blob->GetBufferSize(), l_cachedFile);

		fclose(l_cachedFile);
	};

	if (LoadCachedFile(BlobOut))
	{
		return true;
	}
	else
	{
		hr = D3DX11CompileFromFile(Filename.c_str(), m_ShaderMacros.data(), NULL,
		                           EntryPoint.c_str(), ShaderModel.c_str(),
		                           dwShaderFlags, 0, NULL, BlobOut,
		                           &pErrorBlob, NULL);

		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
			{
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			}

			if (pErrorBlob)
			{
				pErrorBlob->Release();
			}

			return false;
		}

		if (pErrorBlob)
		{
			pErrorBlob->Release();
		}

		SaveFileToCache(*BlobOut);

		return true;
	}
}

bool CEffectShader::CreateConstantBuffer(int IdBuffer, unsigned int BufferSize)
{
	ID3D11Buffer* l_ConstantBuffer;
	CContextManager& l_ContextManager =
	    *CEngine::GetSingleton().getContextManager();
	ID3D11Device* l_Device = l_ContextManager.GetDevice();
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
