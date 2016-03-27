#include "GUI.h"

#include <Core/Engine/Engine.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Graphics/Renderable/RenderableObjectTechnique.h>
#include <Graphics/Mesh/RenderableVertexs.h>

CGUI::CGUI()
	: m_guiComponents( MAX_GUI_ELEMENTS )
	, m_inGUI( false )
{
	m_guiComponentsVtxs = new CPointsListRenderableVertexs<PARTICLE_VERTEX>(m_guiComponents.data(), MAX_GUI_ELEMENTS, MAX_GUI_ELEMENTS, true);
}

CGUI::~CGUI()
{
	delete m_guiComponentsVtxs;
}

void CGUI::Init(CContextManager* cm)
{
	m_contextManager = cm;
}

void CGUI::BeginGUI()
{
	DEBUG_ASSERT( !m_inGUI );

	m_guiComponents.clear();
	m_screen = Rectf( 0, 0, m_contextManager->GetWidth(), m_contextManager->GetHeight() );
	m_nestedPositionStack.clear();
	m_nestedPositionStack.push_back( m_screen );
	m_inGUI = true;;
}

void CGUI::EndGUI()
{
	DEBUG_ASSERT( m_inGUI );
	DEBUG_ASSERT( m_nestedPositionStack.size() == 1 );
	m_inGUI = false;
}

Rectf CGUI::getAligned( const Rectf &r, Alignment alignToParent, Alignment alignSelf )
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

	align( alignedRect.position.x, ( ((uint32)alignToParent) >> 4 ) & 0xf, frame.size.x );
	align( alignedRect.position.y, ((uint32)alignToParent) & 0xf, frame.size.y );

	align( alignedRect.position.x, ( ( (uint32)alignSelf ) >> 4 ) & 0xf, -alignedRect.size.x );
	align( alignedRect.position.y, ((uint32)alignSelf) & 0xf, -alignedRect.size.y );

	return alignedRect;
}

Rectf CGUI::getNormalized( const Rectf &r )
{
	return Rectf( r.x / m_screen.w, r.y / m_screen.h, r.w / m_screen.w, r.h / m_screen.h );
}

void CGUI::Image(const std::string& material, const Rectf& r, Alignment alignToParent, Alignment alignSelf, uint32 sprite)
{
	DEBUG_ASSERT( m_inGUI );

	Rectf imgRect = getAligned( r, alignToParent, alignSelf );

	ImageInternal( material, imgRect, sprite );
}

CGUI::MouseButtonState CGUI::Button( const std::string& material,
									 const Rectf& r,
									 Alignment alignToParent,
									 Alignment alignSelf,
									 uint32 idleSprite,
									 int32 overSprite,
									 int32 downSprite )
{
	Button( material, r, Vect2f( 0, 0 ), alignToParent, alignSelf, idleSprite, overSprite, downSprite );
}

CGUI::MouseButtonState CGUI::Button( const std::string& material,
									 const Rectf& image,
									 const Vect2f& activeAreaSizeOffset,
									 Alignment alignToParent,
									 Alignment alignSelf,
									 uint32 idleSprite,
									 int32 overSprite,
									 int32 downSprite )
{
	DEBUG_ASSERT( m_inGUI );

	if ( overSprite < 0 )
	{
		overSprite = idleSprite + 1;
	}
	if ( downSprite < 0 )
	{
		downSprite = overSprite + 1;
	}

	Rectf imgRect = getAligned( image, alignToParent, alignSelf );

	Rectf activeBound = Rectf( imgRect.position - activeAreaSizeOffset, imgRect.size + activeAreaSizeOffset * 2 );

	MouseButtonState state = getMouseState( activeBound );

	uint32 sprite;
	switch ( state )
	{
		case CGUI::MouseButtonState::UP:
			break;
		case CGUI::MouseButtonState::CLICKED:
			break;
		case CGUI::MouseButtonState::DOWN:
			break;
		case CGUI::MouseButtonState::RELEASED:
			break;
	}
	ImageInternal( material, imgRect, sprite );

}

CGUI::MouseButtonState CGUI::getMouseState( const Rectf& bounds )
{
	return MouseButtonState::UP;
}


void CGUI::ImageInternal( const std::string& material, const Rectf& r, uint32 sprite = 0 )
{
	Rectf imgRect = getNormalized( r );
	m_guiComponents.resize( 1 );
	m_guiComponents[0].Position = Vect3f( imgRect.x, imgRect.y, 0 );

	m_guiComponents[0].Color = CColor( 1, 1, 1, 1 );

	m_guiComponents[0].UV = imgRect.size;

	m_guiComponents[0].UV2.x = sprite;


	m_guiComponentsVtxs->UpdateVertices(m_contextManager->GetDeviceContext(), m_guiComponents.data(), m_guiComponents.size());

	CMaterial* mat = CEngine::GetSingleton().getMaterialManager()->get( material );
	auto technique = mat->getRenderableObjectTechique()->GetEffectTechnique();

	mat->apply();

	m_guiComponentsVtxs->Render(m_contextManager, technique);
}

void CGUI::BeginFrame( const Rectf& r, Alignment alignToParent, Alignment alignSelf )
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
