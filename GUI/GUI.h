#pragma once

#include <Base/Math/Math.h>
#include <Base/Math/Color.h>
#include <Graphics/Mesh/VertexTypes.h>
#include <vector>
#include <map>
#include <string>

#include "./Rect.h"

class CMaterial;
class CXMLTreeNode;
class CContextManager;
class CRenderableVertexs;
class CFont;

class CGUI
{
public:
	static const int MAX_GUI_ELEMENTS = 200;

	enum class Alignment
	{
		TOP_LEFT = 0x11,
		TOP_CENTER = 0x12,
		TOP_RIGHT = 0x14,
		MIDDLE_LEFT = 0x21,
		CENTER = 0x22,
		MIDDLE_RIGHT = 0x24,
		BOTTOM_LEFT = 0x41,
		BOTTOM_CENTER = 0x42,
		BOTTOM_RIGHT = 0x44
	};
	enum class MouseButtonState
	{
		OUTSIDE = 0,
		UP,
		CLICKED,
		DOWN,
		RELEASED
	};

private:
	CContextManager *m_contextManager;

	bool m_inGUI;

	Rectf m_screen;

	std::vector<GUI_VERTEX> m_guiComponents;

	CRenderableVertexs *m_guiComponentsVtxs;

	std::vector<Rectf> m_nestedPositionStack;

	Vect2i m_mousePos;

	std::map<std::string, CMaterial*> m_spriteMats;

	struct SpriteDesc_t
	{
		std::string spritemap;
		Rectf sprite;
	};
	std::map<std::string, SpriteDesc_t> m_sprites;

	std::map<std::string, std::map<std::string, std::string>> m_elements;

	std::map<std::string, CFont*> m_fonts;

public:
	CGUI();
	~CGUI();

	void Init( const std::string& xml );

	void BeginGUI();
	void EndGUI();

	void Image(const std::string& spriteName, const Rectf& r, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT);

	MouseButtonState Button( const std::string& buttonSkin, const Rectf& r, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT);
	MouseButtonState Button( const std::string& buttonSkin, const Rectf& image, const Vect2f& activeAreaSizeOffset, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT);

	float Slider();

	float Text(const std::string &font, const std::string &text);

	void BeginFrame( const Rectf& r, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT );
	void EndFrame();

private:

	void InitSpritemap(CXMLTreeNode &spritemapNode);

	void ImageInternal( const std::string& spriteName, const Rectf& r );

	Rectf getAligned( const Rectf &r, Alignment alignToParent, Alignment alignSelf );
	Rectf getNormalized( const Rectf &r );

	MouseButtonState getMouseState();

	bool isMouseOver(const Rectf& rect);
};
