#pragma once

#include <Base/Utils/Named.h>

#include <vector>
#include <map>

#include "./Rect.h"

class CMaterial;
class CTexture;
class CXMLTreeNode;

class CFont : public CNamed
{
	CMaterial *m_material;

	std::vector<CTexture*> m_pages;

	float m_fonSize;
	float m_lineHeight;
	float m_base;

	struct CharDesc_t
	{
		Vect2f size;
		Vect2f offset;
		float xAdvance;
		Rectf uvRect;
		uint8 page;
	};

	std::map<int32, CharDesc_t> m_chars;

	std::map<std::pair<int32, int32>, int16> m_kernings;
public:

	CFont(CXMLTreeNode& node);
	~CFont();

	void DrawString(const std::string& text, const Rectf& bounds, bool overflowX = false);

	float GetStringWidth(const std::string& text);
};
