#ifndef TEXTURE_H
#define TEXTURE_H

#include <Utils/Named.h>

struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

class CTexture : CNamed
{
private:
	ID3D11ShaderResourceView *m_Texture;
	ID3D11SamplerState *m_SamplerState;

	virtual bool LoadFile();
	void Unload();
public:
	CTexture();
	virtual ~CTexture();
	bool load(const std::string &Filename);
	void Activate(unsigned int StageId);
	bool Reload();
};

#endif