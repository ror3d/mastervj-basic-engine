#ifndef EFFECT_SHADER_H
#define EFFECT_SHADER_H

#include "Utils\Named.h"
#include <d3d11.h>
#include <vector>

class CEffectShader : public CNamed
{
protected:
	std::string m_Filename;
	std::string m_Preprocessor;
	std::string m_ShaderModel;
	std::string m_EntryPoint;

	std::vector<ID3D11Buffer *> m_ConstantBuffers;
	std::vector<std::string> m_PreprocessorMacros;
	D3D10_SHADER_MACRO *m_ShaderMacros;

	bool LoadShader(const std::string &Filename, const std::string &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut);

	void CreateShaderMacro();
	bool CreateConstantBuffer(int IdBuffer, unsigned int BufferSize);
	bool CreateConstantBuffer();
public:
	CEffectShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectShader();
	
	virtual bool Load() = 0;
	virtual void destroy() = 0;

	virtual void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer) = 0;

	virtual bool Reload();
	inline ID3D11Buffer * GetConstantBuffer(unsigned int IdBuffer) const
	{
		return m_ConstantBuffers[IdBuffer];
	}
};

#endif