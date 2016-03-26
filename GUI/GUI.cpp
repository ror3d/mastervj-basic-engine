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

void CGUI::Image( const Rectf& r, const std::string& material, uint32 sprite, Alignment alignToParent, Alignment alignSelf )
{
	DEBUG_ASSERT( m_inGUI );

	Rectf imgRect = getNormalized(getAligned( r, alignToParent, alignSelf ));

	/*
	m_contextManager->DrawScreenQuad( material->getRenderableObjectTechique()->GetEffectTechnique(),
		material->GetTexture(0), imgRect.x, imgRect.y, imgRect.w, imgRect.h, CColor(1, 1, 1, 1) );
		*/
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
