#include "Font.h"
#include "GUI.h"

#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Graphics/Renderable/RenderableObjectTechnique.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Texture/Texture.h>
#include <Graphics/Mesh/RenderableVertexs.h>
#include <Graphics/Effect/EffectManager.h>


CFont::CFont(CXMLTreeNode& fontNode, CGUI* gui)
	: CNamed(fontNode)
	, m_glyphs( MAX_GLYPHS_BATCH )
	, m_gui(gui)
	, m_textAlign(Rectf::Alignment::TOP_LEFT)
{
	auto mm = CEngine::GetSingleton().getMaterialManager();
	auto tm = CEngine::GetSingleton().getTextureManager();

	std::string fontFile = fontNode.GetPszProperty("path", "", false);
	std::string fontName = fontNode.GetPszProperty("name", "", false);
	DEBUG_ASSERT(fontFile.size() != 0 && fontName.size() != 0);
	if (fontFile.size() == 0 || fontName.size() == 0)
	{
		return;
	}

	for (int i = 0; i < fontNode.GetNumChildren(); ++i)
	{
		auto matNode = fontNode(i);
		if (matNode.GetName() == std::string("material"))
		{
			std::string matName = "font-material-" + fontName;
			CMaterial *mat = new CMaterial(matNode);
			mat->setName(matName);
			mm->add(matName, mat);
			m_material = mat;
			break;
		}
	}

	std::string fontPath;
	size_t pathEnd = fontFile.find_last_of('\\');
	if (pathEnd == fontFile.npos)
	{
		pathEnd = fontFile.find_last_of('/');
	}

	if (pathEnd != fontFile.npos)
	{
		fontPath = fontFile.substr(0, pathEnd+1);
	}

	CXMLTreeNode ff;
	if (!ff.LoadFile(fontFile.c_str()))
	{
		DEBUG_ASSERT(false);
		return;
	}

	CXMLTreeNode font = ff["font"];

	CXMLTreeNode common = font["common"];

	CXMLTreeNode info = font["info"];

	Vect2f pageSize(common.GetFloatProperty("scaleW", 0, false), common.GetFloatProperty("scaleH", 0, false));

	m_fontSize = info.GetFloatProperty( "size", 0, false );

	DEBUG_ASSERT( m_fontSize != 0);

	m_lineHeight = common.GetFloatProperty( "lineHeight", m_fontSize, false );

	m_base = common.GetFloatProperty( "base", m_fontSize, false );

	CXMLTreeNode pages = font["pages"];

	for (int i = 0; i < pages.GetNumChildren(); ++i)
	{
		auto page = pages(i);
		if (page.GetName() == std::string("page"))
		{
			std::string texFile = page.GetPszProperty("file", "", false);
			DEBUG_ASSERT(texFile.size() > 0);
			texFile = fontPath + texFile;
			CTexture *tex = new CTexture();
			tex->load(texFile, false);
			tm->add(tex->getName(), tex);
			m_pages.push_back(tex);
		}
	}

	auto chars = font["chars"];

	for (int i = 0; i < chars.GetNumChildren(); ++i)
	{
		auto ch = chars(i);
		if (ch.GetName() != std::string("char"))
		{
			continue;
		}
		uchar chId = ch.GetIntProperty("id");

		CharDesc_t cdesc;
		cdesc.offset = Vect2f(ch.GetFloatProperty("xoffset", 0, false), ch.GetFloatProperty("yoffset", 0, false));
		cdesc.page = ch.GetIntProperty("page");
		cdesc.size = Vect2f(ch.GetFloatProperty("width", 0, false), ch.GetFloatProperty("height", 0, false));
		cdesc.xAdvance = ch.GetFloatProperty("xadvance", 0, false);

		cdesc.uvRect.position = Vect2f(ch.GetFloatProperty("x", 0, false), ch.GetFloatProperty("y", 0, false));
		cdesc.uvRect.position = Vect2f(cdesc.uvRect.position.x / pageSize.x, cdesc.uvRect.position.y / pageSize.y);
		cdesc.uvRect.size = Vect2f(cdesc.size.x / pageSize.x, cdesc.size.y / pageSize.y);
		m_chars[chId] = cdesc;
	}

	auto kerns = font["kernings"];

	if (kerns.Exists())
	{
		for (int i = 0; i < kerns.GetNumChildren(); ++i)
		{
			auto k = kerns(i);
			uchar f = k.GetIntProperty("first", 0, false);
			uchar s = k.GetIntProperty("second", 0, false);
			uchar a = k.GetIntProperty("amount", 0, false);

			m_kernings[std::make_pair(f, s)] = a;
		}
	}

	m_glyphsVtxs = new CPointsListRenderableVertexs<GUI_TEXT_VERTEX>(m_glyphs.data(), MAX_GLYPHS_BATCH, MAX_GLYPHS_BATCH, true);
}

CFont::~CFont()
{
}

void CFont::SetColor( const CColor & color )
{
	m_color = color;
}

void CFont::DrawString(const std::string& text, const Rectf& bounds, bool overflowX /*= false*/)
{
	ustring str = GetUTF8String( text );
	Vect2f screenSz = m_gui->m_screen.size;
	Vect2f pos = bounds.position;

	pos.x = GetLineStartXPosition(str, bounds, 0);

	CEffectManager::m_SceneParameters.m_BaseColor = m_color;

	auto technique = m_material->getRenderableObjectTechique()->GetEffectTechnique();
	m_material->apply();
	m_pages[0]->Activate(0);

	for ( int b = 0; b < str.length(); b += MAX_GLYPHS_BATCH )
	{
		m_glyphs.clear();

		uchar lastChar = -1;
		for ( int i = b; (i - b < MAX_GLYPHS_BATCH) && (i < str.length()); ++i )
		{
			uchar c = str[i];
			if (c == '\n')
			{
				lastChar = -1;
				pos.x = GetLineStartXPosition(str, bounds, i+1);
				pos.y += m_lineHeight;
				continue;
			}
			auto descIt = m_chars.find( c );

			if ( descIt == m_chars.end() )
			{
				c = -1;
				descIt = m_chars.find( -1 );
			}

			if ( lastChar == -1 )
			{
				auto kernIt = m_kernings.find( std::make_pair( lastChar, c ) );
				if ( kernIt != m_kernings.end() )
				{
					pos.x += kernIt->second;
				}
			}
			auto & desc = descIt->second;

			Rectf charR( pos, desc.size );

			charR.position += desc.offset;

			charR.x /= screenSz.x;
			charR.y /= screenSz.y;
			charR.w /= screenSz.x;
			charR.h /= screenSz.y;

			m_glyphs.push_back( {Vect4f(charR.x, charR.y, charR.w, charR.h), desc.uvRect.position, desc.uvRect.size } );


			pos.x += desc.xAdvance;

			lastChar = c;
		}

		m_glyphsVtxs->UpdateVertices(m_gui->m_contextManager->GetDeviceContext(), m_glyphs.data(), m_glyphs.size());

		m_glyphsVtxs->Render(m_gui->m_contextManager, technique);
	}
}

float CFont::GetLineWidth(const ustring& text, size_t start /*= 0*/)
{
	size_t count = text.length()-start;
	for (int i = 0; i+start < text.size(); ++i)
	{
		if (text[i+start] == '\n')
		{
			count = i;
			break;
		}
	}
	return GetStringWidth(text.substr(start, count));
}

float CFont::GetLineStartXPosition(const ustring& text, const Rectf& bounds, size_t start /*= 0*/)
{
	float w = GetLineWidth(text, start);
	float s = bounds.x;
	switch ((unsigned int(m_textAlign)) & 0x0f)
	{
		case 0x02:
			s = s + bounds.w/2 - w / 2;
			break;
		case 0x04:
			s = s + bounds.w - w;
			break;
		default:
			break;
	}
	return std::roundf(s);
}

float CFont::GetStringWidth(const std::string& text)
{
	return GetStringWidth( GetUTF8String( text ) );
}

CFont::ustring CFont::GetUTF8String( const std::string & text )
{
	ustring str;
	for ( int i = 0; i < text.length(); ++i )
	{
		uchar uc = 0;
		char c = text[i];
		if ( c & 0x80 )
		{
			int n;
			if ( (c & 0xe0) == 0xc0 )
			{
				n = 1;
				c = c & 0x1f;
			}
			else if ( (c & 0xf0) == 0xe0 )
			{
				n = 2;
				c = c & 0x0f;
			}
			else if ( (c & 0xf8) == 0xf0 )
			{
				n = 3;
				c = c & 0x07;
			}
			else if ( (c & 0xfc) == 0xf8 )
			{
				n = 4;
				c = c & 0x03;
			}
			else if ( (c & 0xfe) == 0xfc )
			{
				n = 5;
				c = c & 0x01;
			}
			else
			{
				DEBUG_ASSERT( false );
			}
			uc = c << ( 6 * n );
			n--;
			for ( ; n >= 0; --n )
			{
				i++;
				DEBUG_ASSERT( i < text.length() );
				c = text[i];
				DEBUG_ASSERT( (c & 0xc0) == 0x80 );
				c = c & 0x3f;
				uc = uc | ( c << ( 6 * n ) );
			}
		}
		else
		{
			uc = c;
		}
		str += uc;
	}
	return str;
}

float CFont::GetStringWidth( const ustring& text, bool offsetFirstCharacter /*= false*/ )
{
	float totalW = 0;

	uchar lastChar = 0;
	for ( int i = 0; i < text.length(); ++i )
	{
		uchar c = text[i];
		auto descIt = m_chars.find(c);

		if ( descIt == m_chars.end() )
		{
			descIt = m_chars.find( -1 );
		}
		auto & desc = descIt->second;

		if (i != 0 || offsetFirstCharacter)
		{
			totalW += desc.offset.x;
		}
		totalW += desc.xAdvance;

		auto kernIt = m_kernings.find( std::make_pair( lastChar, c ) );
		if ( kernIt != m_kernings.end() )
		{
			totalW += kernIt->second;
		}

		lastChar = c;
	}


	return totalW;
}
