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

	enum class MouseButtonState
	{
		OUTSIDE = 0,
		UP,
		CLICKED,
		DOWN,
		RELEASED
	};

private:
	static CGUI* m_instance;

	CContextManager *m_contextManager;

	bool m_inGUI;

	Rectf m_screen;

	std::vector<Rectf> m_nestedPositionStack;


	std::vector<GUI_VERTEX> m_guiComponents;

	CRenderableVertexs *m_guiComponentsVtxs;


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

	friend class CFont;
public:
	CGUI();
	~CGUI();

	static inline CGUI* GetInstance() { return m_instance; }
	static void SetInstance(CGUI* gui) { m_instance = gui; }

	void Init( const std::string& xml );

	void BeginGUI();
	void EndGUI();

	void Image(const std::string& spriteName, const Rectf& r, Rectf::Alignment alignToParent = Rectf::Alignment::TOP_LEFT, Rectf::Alignment alignSelf = Rectf::Alignment::TOP_LEFT);

	MouseButtonState Button( const std::string& buttonSkin, const Rectf& r, Rectf::Alignment alignToParent = Rectf::Alignment::TOP_LEFT, Rectf::Alignment alignSelf = Rectf::Alignment::TOP_LEFT);
	MouseButtonState Button( const std::string& buttonSkin, const Rectf& image, const Vect2f& activeAreaSizeOffset, Rectf::Alignment alignToParent = Rectf::Alignment::TOP_LEFT, Rectf::Alignment alignSelf = Rectf::Alignment::TOP_LEFT);

	float Slider();

	void Text(const std::string &font, const std::string &text, const Rectf& bounds, Rectf::Alignment alignToParent = Rectf::Alignment::TOP_LEFT, Rectf::Alignment alignSelf = Rectf::Alignment::TOP_LEFT, bool overflowX = true );

	void BeginFrame( const Rectf& r, Rectf::Alignment alignToParent = Rectf::Alignment::TOP_LEFT, Rectf::Alignment alignSelf = Rectf::Alignment::TOP_LEFT );
	void EndFrame();

private:

	void InitSpritemap(CXMLTreeNode &spritemapNode);

	void ImageInternal( const std::string& spriteName, const Rectf& r );

	Rectf getAligned( const Rectf &r, Rectf::Alignment alignToParent, Rectf::Alignment alignSelf );
	Rectf getNormalized( const Rectf &r );

	MouseButtonState getMouseState();

	bool isMouseOver(const Rectf& rect);
};
