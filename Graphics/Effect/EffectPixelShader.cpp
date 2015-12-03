#include "EffectPixelShader.h"
#include <Engine/Engine.h>


CEffectPixelShader::CEffectPixelShader(const CXMLTreeNode &TreeNode) : CEffectShader(TreeNode)
{
	Load();
}


CEffectPixelShader::~CEffectPixelShader()
{
}


bool CEffectPixelShader::Load()
{
	ID3DBlob *l_PSBlob = NULL;
	bool l_Loaded = LoadShader(m_Filename.c_str(), m_EntryPoint.c_str(), m_ShaderModel.c_str(), &l_PSBlob);
	if (!l_Loaded)
		return false;
	ID3D11Device *l_Device = CEngine::GetSingleton().getContextManager()->GetDevice();
	HRESULT l_HR = l_Device->CreatePixelShader(l_PSBlob->GetBufferPointer(), l_PSBlob->GetBufferSize(), NULL, &m_PixelShader);
	l_PSBlob->Release();
	return CreateConstantBuffer();
}


void CEffectPixelShader::Destroy()
{
	int i = 0;
}
