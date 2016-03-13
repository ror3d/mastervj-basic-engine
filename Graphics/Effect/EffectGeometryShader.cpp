#include "EffectGeometryShader.h"
#include <Engine/Engine.h>


CEffectGeometryShader::CEffectGeometryShader(const CXMLTreeNode &TreeNode)
	: CEffectShader(TreeNode)
	, m_GeometryShader(nullptr)
{
}


CEffectGeometryShader::~CEffectGeometryShader()
{
	CHECKED_RELEASE( m_GeometryShader );
}

bool CEffectGeometryShader::Load()
{
	CreateShaderMacro();
	ID3DBlob *l_PSBlob = NULL;
	bool l_Loaded = LoadShader(m_Filename.c_str(), m_EntryPoint.c_str(),
							   m_ShaderModel.c_str(), &l_PSBlob);
	if (!l_Loaded)
		return false;
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	HRESULT l_HR = l_Device->CreateGeometryShader(l_PSBlob->GetBufferPointer(),
											   l_PSBlob->GetBufferSize(), NULL, &m_GeometryShader);
	l_PSBlob->Release();
	return CreateConstantBuffer();
}
void CEffectGeometryShader::SetConstantBuffer(unsigned int IdBuffer, void
										   *ConstantBuffer)
{
	ID3D11DeviceContext *l_DeviceContext = CEngine::GetSingleton().getContextManager()->GetDeviceContext();
	ID3D11Buffer *l_ConstantBuffer = GetConstantBuffer(IdBuffer);
	if (l_ConstantBuffer)
	{
		l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL,
										   ConstantBuffer, 0, 0);
		l_DeviceContext->PSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer);
	}
}

void CEffectGeometryShader::destroy()
{

}
