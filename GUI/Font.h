#pragma once

#include <Base/Utils/Named.h>
#include <Base/Math/Color.h>
#include <Graphics/Mesh/VertexTypes.h>

#include <vector>
#include <unordered_map>

#include "./Rect.h"

class CMaterial;
class CTexture;
class CXMLTreeNode;
class CRenderableVertexs;
class CGUI;

namespace std
{
	template<> struct hash<std::pair<uint32, uint32>>
	{
		typedef std::pair<uint32, uint32> argument_type;
		typedef std::size_t result_type;
		result_type operator()( argument_type const& s ) const
		{
			result_type const h1( std::hash<uint32>()( s.first ) );
			result_type const h2( std::hash<uint32>()( s.second ) );
			return h1 ^ ( h2 << 1 ); // or use boost::hash_combine
		}
	};
}

class CFont : public CNamed
{
	static const int MAX_GLYPHS_BATCH = 500;

	typedef std::u32string ustring;
	typedef uint32 uchar;

	CGUI * m_gui;

	CMaterial *m_material;

	std::vector<CTexture*> m_pages;

	float m_fontSize;
	float m_lineHeight;
	float m_base;

	CColor m_color;

	struct CharDesc_t
	{
		Vect2f size;
		Vect2f offset;
		float xAdvance;
		Rectf uvRect;
		uint8 page;
	};

	std::unordered_map<uint32, CharDesc_t> m_chars;

	std::unordered_map < std::pair<uint32, uint32>, int16> m_kernings;

	std::vector<GUI_TEXT_VERTEX> m_glyphs;

	CRenderableVertexs *m_glyphsVtxs;


	ustring GetUTF8String( const std::string& text );

	float GetStringWidth( const ustring& text );
public:

	CFont(CXMLTreeNode& node, CGUI* gui);
	~CFont();

	void SetColor( const CColor& color );

	void DrawString(const std::string& text, const Rectf& bounds, bool overflowX = false);

	float GetStringWidth(const std::string& text);
};
