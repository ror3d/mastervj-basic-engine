#include "EffectVertexShader.h"
#include "Engine/Engine.h"
#include "VertexTypes.h"


CEffectVertexShader::CEffectVertexShader(const CXMLTreeNode &TreeNode)
	: CEffectShader(TreeNode)
{
	m_VertexType = TreeNode.GetPszProperty("vertex_type");
	Load();
}


CEffectVertexShader::~CEffectVertexShader()
{
}


bool CEffectVertexShader::Load()
{
	ID3DBlob *l_VSBlob = NULL;
	bool l_Loaded = LoadShader(m_Filename, m_EntryPoint, m_ShaderModel, &l_VSBlob);
	if (!l_Loaded)
		return false;
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	HRESULT l_HR = l_Device->CreateVertexShader(l_VSBlob->GetBufferPointer(), l_VSBlob->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(l_HR))
	{
		l_VSBlob->Release();
		return false;
	}
	if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_VERTEX")
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	else if (m_VertexType == "MV_POSITION_COLOR_VERTEX")
		l_Loaded = MV_POSITION_COLOR_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	else
		//Info("Vertex type '%s' not recognized on CEffectVertexShader::Load", m_VertexType.c_str());
	l_VSBlob->Release();
	if (!l_Loaded)
		return false;
	return CreateConstantBuffer();
}


void CEffectVertexShader::Destroy()
{
	int i = 0;
}