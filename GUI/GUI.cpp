#include "GUI.h"

#include "Font.h"

#include <Core/Engine/Engine.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Graphics/Renderable/RenderableObjectTechnique.h>
#include <Graphics/Mesh/RenderableVertexs.h>

#include <Core/Input/InputManager.h>

#include <Base/XML/XMLTreeNode.h>

CGUI* CGUI::m_instance = nullptr;

CGUI::CGUI()
	: m_guiComponents( MAX_GUI_ELEMENTS )
	, m_inGUI( false )
	, m_fontColor(0, 0, 0, 1)
{
	m_guiComponentsVtxs = new CPointsListRenderableVertexs<GUI_VERTEX>(m_guiComponents.data(), MAX_GUI_ELEMENTS, MAX_GUI_ELEMENTS, true);
}

CGUI::~CGUI()
{
	delete m_guiComponentsVtxs;
	// TODO: Cleanup all the maps
}

void CGUI::Init(const std::string& xml)
{
	m_contextManager = CEngine::GetSingleton().getContextManager();

	CXMLTreeNode xf;
	if (xf.LoadFile(xml.c_str()))
	{
		DEBUG_ASSERT(xf.GetName() == std::string("gui_elements"));

		for (int i = 0; i < xf.GetNumChildren(); ++i)
		{
			auto elem = xf(i);
			std::string elemTag(elem.GetName());

			if (elemTag == "spritemap")
			{
				InitSpritemap(elem);
			}
			else if (elemTag == "font")
			{
				CFont *font = new CFont(elem, this);
				m_fonts[font->getName()] = font;
			}
			else
			{
				auto ps = elem.GetProperties();
				std::string elemName = elem.GetPszProperty("name", "", false);
				DEBUG_ASSERT(elemName.size() != 0);
				if (elemName.size() == 0) continue;
				std::map<std::string, std::string> props;
				props["type"] = elemTag;
				for (auto const &p : ps)
				{
					props[p] = elem.GetPszProperty(p.c_str(), "", false);
				}
				m_elements[elemName] = std::move(props);
			}
		}
	}
}

void CGUI::InitSpritemap(CXMLTreeNode &spritemapNode)
{
	auto mm = CEngine::GetSingleton().getMaterialManager();
	std::string spritemapName = spritemapNode.GetPszProperty("name");
	int sWidth = spritemapNode.GetIntProperty("width");
	int sHeight = spritemapNode.GetIntProperty("height");
	for (int j = 0; j < spritemapNode.GetNumChildren(); ++j)
	{
		auto spItem = spritemapNode(j);
		std::string spTag(spItem.GetName());
		if (spTag == "material")
		{
			CMaterial *mat = new CMaterial(spItem);
			mm->add(mat->getName(), mat);
			m_spriteMats[spritemapName] = mat;
		}
		else if (spTag == "sprite")
		{
			std::string spName = spItem.GetPszProperty("name", "", false);
			DEBUG_ASSERT(spName.size() != 0);
			if (spName.size() == 0) continue;
			SpriteDesc_t sprite;
			sprite.spritemap = spritemapName;
			Vect4f q = spItem.GetVect4fProperty("xywh", Vect4f(), false);
			sprite.sprite = Rectf(q.x/sWidth, q.y/sHeight, q.z/sWidth, q.w/sHeight);
			m_sprites[spName] = sprite;
		}
	}
}

void CGUI::BeginGUI()
{
	DEBUG_ASSERT( !m_inGUI );

	m_guiComponents.clear();
	m_screen = Rectf( 0, 0, m_contextManager->GetWidth(), m_contextManager->GetHeight() );
	m_nestedPositionStack.clear();
	m_nestedPositionStack.push_back( m_screen );
	m_mousePos = CInputManager::GetInputManager()->GetCursor();
	m_inGUI = true;
}

void CGUI::EndGUI()
{
	DEBUG_ASSERT( m_inGUI );
	DEBUG_ASSERT( m_nestedPositionStack.size() == 1 );
	m_inGUI = false;
}

Rectf CGUI::getAligned( const Rectf &r, Rectf::Alignment alignToParent, Rectf::Alignment alignSelf )
{
	Rectf alignedRect(r);

	Rectf frame = m_nestedPositionStack.back();

	if ( r.relative )
	{
		alignedRect.x *= frame.w;
		alignedRect.y *= frame.h;
		alignedRect.w *= frame.w;
		alignedRect.h *= frame.h;
	}

	alignedRect.position += frame.position;

	auto align = []( float & value, uint32 alignment, float parentSize ) {
		switch ( alignment )
		{
			case 0x1:
				// Nothing
				break;
			case 0x2:
				value += parentSize / 2;
				break;
			case 0x4:
				value += parentSize;
				break;
		}
	};

	align( alignedRect.position.y, ( ((uint32)alignToParent) >> 4 ) & 0xf, frame.size.y );
	align( alignedRect.position.x, ( (uint32)alignToParent) & 0xf, frame.size.x );

	align( alignedRect.position.y, ( ((uint32)alignSelf) >> 4 ) & 0xf, -alignedRect.size.y );
	align( alignedRect.position.x, ( (uint32)alignSelf ) & 0xf, -alignedRect.size.x );

	return alignedRect;
}

Rectf CGUI::getNormalized( const Rectf &r )
{
	Rectf n( r.x / m_screen.w, r.y / m_screen.h, r.w / m_screen.w, r.h / m_screen.h );
	n.position = n.position * 2 - Vect2f(1, 1);
	n.size *= 2;
	n.position.y = -n.position.y - n.size.y;
	return n;
}

void CGUI::Image(const std::string& spriteName, const Rectf& r, Rectf::Alignment alignToParent, Rectf::Alignment alignSelf)
{
	DEBUG_ASSERT( m_inGUI );

	Rectf imgRect = getAligned( r, alignToParent, alignSelf );

	ImageInternal( spriteName, imgRect );
}

CGUI::MouseButtonState CGUI::Button( const std::string& buttonSkin,
									 const Rectf& r,
									 Rectf::Alignment alignToParent,
									 Rectf::Alignment alignSelf )
{
	return Button( buttonSkin, r, Vect2f( 0, 0 ), alignToParent, alignSelf );
}

CGUI::MouseButtonState CGUI::Button( const std::string& buttonSkin,
									 const Rectf& image,
									 const Vect2f& activeAreaSizeOffset,
									 Rectf::Alignment alignToParent,
									 Rectf::Alignment alignSelf )
{
	DEBUG_ASSERT( m_inGUI );

	Rectf imgRect = getAligned( image, alignToParent, alignSelf );

	Rectf activeBound = Rectf( imgRect.position - activeAreaSizeOffset, imgRect.size + activeAreaSizeOffset * 2 );

	auto buttonDesc = m_elements[buttonSkin];

	DEBUG_ASSERT(buttonDesc["type"] == "button");

	std::string sprite = buttonDesc["normal"];
	CGUI::MouseButtonState rstate = MouseButtonState::OUTSIDE;
	if (isMouseOver(activeBound))
	{
		MouseButtonState state = getMouseState();

		switch (state)
		{
			case CGUI::MouseButtonState::CLICKED:
			case CGUI::MouseButtonState::DOWN:
			case CGUI::MouseButtonState::RELEASED:
				sprite = buttonDesc["pressed"];
				break;
			case CGUI::MouseButtonState::UP:
				sprite = buttonDesc["over"];
				break;
		}
		rstate = state;
	}
	ImageInternal( sprite, imgRect );

	return rstate;
}

CGUI::MouseButtonState CGUI::getMouseState()
{
	if (CInputManager::GetInputManager()->IsActionActive("MOUSE_PRESSED"))
	{
		return MouseButtonState::CLICKED;
	}
	if (CInputManager::GetInputManager()->IsActionActive("MOUSE_DOWN"))
	{
		return MouseButtonState::DOWN;
	}
	if (CInputManager::GetInputManager()->IsActionActive("MOUSE_RELEASED"))
	{
		return MouseButtonState::RELEASED;
	}
	return MouseButtonState::UP;
}

bool CGUI::isMouseOver(const Rectf& rect)
{
	Vect2i mouse = m_mousePos;
	if (mouse.x >= rect.x && mouse.x <= rect.x + rect.w
		&& mouse.y >= rect.y && mouse.y <= rect.y + rect.h)
	{
		return true;
	}
	return false;
}


void CGUI::ImageInternal( const std::string& spriteName, const Rectf& r )
{
	Rectf imgRect = getNormalized( r );
	m_guiComponents.resize( 1 );
	m_guiComponents[0].Position = Vect3f( imgRect.x, imgRect.y, 0 );

	m_guiComponents[0].UV = imgRect.size;

	auto &sprite = m_sprites[spriteName];

	m_guiComponents[0].Color = CColor( sprite.sprite.x, sprite.sprite.y, sprite.sprite.w, sprite.sprite.h );


	m_guiComponentsVtxs->UpdateVertices(m_contextManager->GetDeviceContext(), m_guiComponents.data(), m_guiComponents.size());

	CMaterial* mat = m_spriteMats[sprite.spritemap];
	auto technique = mat->getRenderableObjectTechique()->GetEffectTechnique();

	mat->apply();

	m_guiComponentsVtxs->Render(m_contextManager, technique);
}

void CGUI::BeginFrame( const Rectf& r, Rectf::Alignment alignToParent, Rectf::Alignment alignSelf )
{
	DEBUG_ASSERT( m_inGUI );

	Rectf frameR = getAligned( r, alignToParent, alignSelf );
	m_nestedPositionStack.push_back( frameR );
}

void CGUI::EndFrame()
{
	DEBUG_ASSERT( m_inGUI );
	DEBUG_ASSERT( m_nestedPositionStack.size() > 1 );

	m_nestedPositionStack.pop_back();
}

void CGUI::Text(const std::string &fontName, const std::string &text, const Rectf& bounds, Rectf::Alignment alignToParent /*= Rectf::Alignment::TOP_LEFT*/, Rectf::Alignment alignSelf /*= Rectf::Alignment::TOP_LEFT*/, bool overflowX /*= true*/)
{
	DEBUG_ASSERT(m_fonts.find(fontName) != m_fonts.end());

	auto font = m_fonts[fontName];

	font->SetColor(m_fontColor);
	font->SetAlignment(alignSelf);
	font->DrawString(text, getAligned(bounds, alignToParent, alignSelf), overflowX);
}
