#include "EffectVertexShader.h"
#include "Engine/Engine.h"
#include "Mesh/VertexTypes.h"
#include <Graphics/Context/ContextManager.h>
#include <Base/XML/XMLTreeNode.h>


CEffectVertexShader::CEffectVertexShader(const CXMLTreeNode &TreeNode)
	: CEffectShader(TreeNode)
	, m_VertexShader(nullptr)
	, m_VertexLayout(nullptr)
{
	m_VertexType = TreeNode.GetPszProperty("vertex_type");
}


CEffectVertexShader::~CEffectVertexShader()
{
	CHECKED_RELEASE( m_VertexShader );
	CHECKED_RELEASE( m_VertexLayout );
}


bool CEffectVertexShader::Load()
{
	CreateShaderMacro();
	ID3DBlob *l_VSBlob = NULL;
	bool l_Loaded = LoadShader(m_Filename, m_EntryPoint, m_ShaderModel,
							   &l_VSBlob);
	DEBUG_ASSERT(l_Loaded);
	if (!l_Loaded)
	{
		return false;
	}
	CContextManager &l_context = *CEngine::GetSingleton().getContextManager();
	ID3D11Device *l_Device = l_context.GetDevice();
	HRESULT l_HR = l_Device->CreateVertexShader(l_VSBlob->GetBufferPointer(),
												l_VSBlob->GetBufferSize(), NULL,
												&m_VertexShader);
	DEBUG_ASSERT(!FAILED(l_HR));
	if (FAILED(l_HR))
	{
		l_VSBlob->Release();
		return false;
	}
	if (m_VertexType == "MV_POSITION_TEXTURE_VERTEX")
	{
		l_Loaded = MV_POSITION_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_VERTEX")
	{
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION4_COLOR_TEXTURE_VERTEX")
	{
		l_Loaded = MV_POSITION4_COLOR_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX")
	{
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX")
	{
		l_Loaded = MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_TEXTURE2_VERTEX")
	{
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_BINORMAL_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_WEIGHT_INDICES_VERTEX")
	{
		l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_WEIGHT_INDICES_VERTEX")
	{
		l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_TEXTURE2_WEIGHT_INDICES_VERTEX")
	{
		l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_TANGENT_BINORMAL_TEXTURE_TEXTURE2_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_BINORMAL_VERTEX")
	{
		l_Loaded = MV_POSITION_NORMAL_TEXTURE_TEXTURE2_TANGENT_BINORMAL_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}

	else if (m_VertexType == "MV_POSITION_COLOR_VERTEX")
	{
		l_Loaded = MV_POSITION_COLOR_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_WEIGHT_INDICES_VERTEX")
	{
		l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_TEXTURE2_WEIGHT_INDICES_VERTEX")
	{
		l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_TANGENT_BINORMAL_TEXTURE_TEXTURE2_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "PARTICLE_VERTEX")
	{
		l_Loaded = PARTICLE_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "GUI_VERTEX")
	{
		l_Loaded = GUI_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else if (m_VertexType == "GUI_TEXT_VERTEX")
	{
		l_Loaded = GUI_TEXT_VERTEX::CreateInputLayout(l_Device, l_VSBlob, &m_VertexLayout);
	}
	else
	{
		DEBUG_ASSERT(!"Vertex type '%s' not recognized on CEffectVertexShader::Load");
	}
	l_VSBlob->Release();
	DEBUG_ASSERT(l_Loaded);
	if (!l_Loaded)
		return false;
	return CreateConstantBuffer();
}

void CEffectVertexShader::SetConstantBuffer(unsigned int IdBuffer, void
											*ConstantBuffer)
{
	ID3D11DeviceContext
		*l_DeviceContext = CEngine::GetSingleton().getContextManager()->GetDeviceContext();
	ID3D11Buffer *l_ConstantBuffer = GetConstantBuffer(IdBuffer);
	if (l_ConstantBuffer != NULL)
	{
		l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL,
										   ConstantBuffer, 0, 0);
		l_DeviceContext->VSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer);
	}
}


void CEffectVertexShader::destroy()
{
	CHECKED_RELEASE( m_VertexShader );
	CHECKED_RELEASE( m_VertexLayout );
}
