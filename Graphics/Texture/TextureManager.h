#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <Base/Utils/TMapManager.h>
#include "Texture.h"

class CTextureManager : public TMapManager<CTexture>
{
public:
	enum class SamplerStateType
	{
		LinearFilter_WrapEdges,
		NoFilter_WrapEdges,
		LinearFilter_ClampEdges,
		NoFilter_ClampEdges
	};

private:
	std::map<int, CTexture*> m_activeTextures;
	std::map<SamplerStateType, ID3D11SamplerState*> m_samplerStates;

public:
	CTextureManager();
	virtual ~CTextureManager();

	void Init();

	CTexture * GetTexture(const std::string &Filename);
	void Reload();

	void SetTextureAtStage(int stage, CTexture* texture) { m_activeTextures[stage] = texture; }
	CTexture* GetTextureAtStage(int stage) { return m_activeTextures[stage]; }
	void DeactivateTextures();
	bool isEmpty() { return (m_resources.size() == 0); }


	ID3D11SamplerState * GetSamplerState( SamplerStateType type ) { return m_samplerStates[type]; }

private:
	void CreateSamplerState( SamplerStateType type, bool linearFilter, bool wrapEdges );
};

#endif