#include "Font.h"

#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Graphics/Texture/TextureManager.h>
#include <Graphics/Texture/Texture.h>


CFont::CFont(CXMLTreeNode& fontNode)
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

	Vect2f pageSize(common.GetFloatProperty("scaleW", 0, false), common.GetFloatProperty("scaleH", 0, false));


	CXMLTreeNode pages = font["pages"];

	for (int i = 0; i < pages.GetNumChildren(); ++i)
	{
		auto page = pages(i);
		if (page.GetName() == std::string("page"))
		{
			std::string texFile = page.GetPszProperty("file", "", false);
			DEBUG_ASSERT(texFile.size() > 0);
			texFile = fontPath + texFile;
			CTexture *tex = new CTexture(texFile);
			tex->Reload();
			tm->add(tex->getName(), tex);
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
		int32 chId = ch.GetIntProperty("id");

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

	for (int i = 0; i < kerns.GetNumChildren(); ++i)
	{
		auto k = kerns(i);
		int32 f = k.GetIntProperty("first", 0, false);
		int32 s = k.GetIntProperty("second", 0, false);
		int32 a = k.GetIntProperty("amount", 0, false);

		m_kernings[std::make_pair(f, s)] = a;
	}
}

CFont::~CFont()
{
}

void CFont::DrawString(const std::string& text, const Rectf& bounds, bool overflowX /*= false*/)
{
}

float CFont::GetStringWidth(const std::string& text)
{
}
