#ifndef CAPTURE_FRAME_BUFFER_TEXTURE_H
#define CAPTURE_FRAME_BUFFER_TEXTURE_H

#include "Texture\Texture.h"
#include <D3D11.h>

class CCaptureFrameBufferTexture : public CTexture
{
protected:
	ID3D11Texture2D *m_DataTexture;
	void Init(unsigned int Width, unsigned int Height);
	void Unload();
	bool CreateSamplerState();
	UINT m_Width;
	UINT m_Height;
public:
	bool Reload();
	CCaptureFrameBufferTexture(const CXMLTreeNode &TreeNode);
	CCaptureFrameBufferTexture(const std::string &Name, unsigned int Width,
		unsigned int Height);
	virtual ~CCaptureFrameBufferTexture();
	bool Capture(unsigned int StageId);
};

#endif