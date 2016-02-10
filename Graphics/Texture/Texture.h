#ifndef TEXTURE_H
#define TEXTURE_H

#include <Utils/Named.h>

struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

class CTexture : public CNamed
{
private:
	ID3D11ShaderResourceView *m_Texture;
	ID3D11SamplerState *m_SamplerState;

	virtual bool LoadFile();
	void Unload();
public:
	CTexture();
	CTexture(const std::string& name);
	virtual ~CTexture();
	bool load(const std::string &Filename);
	void Activate(unsigned int StageId);
	bool Reload();
	ID3D11SamplerState ** GetSamplerState(){ return &m_SamplerState;  }
	ID3D11ShaderResourceView ** GetShaderResourceView(){ return &m_Texture;  }
};

#endif