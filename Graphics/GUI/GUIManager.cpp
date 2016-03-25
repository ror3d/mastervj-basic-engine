#include "GUIManager.h"
#include "GUIContainer.h"
#include "GUIRenderer.h"

/** Example XML page

<?xml version="1.0" encoding="ISO-8859-1"?>
<page name="main-menu" base-size="1920 1080" size="100% 100%">
	<frame size="100 100" offset="50 50">
		<image size="50% 50%" offset="50% 50%" pivot="50% 50%" />
	</frame>
</page>

*/


CGUIManager::CGUIManager()
{
	m_renderer = new CGUIRenderer();
}

CGUIManager::~CGUIManager()
{
	delete m_renderer;
}

void CGUIManager::LoadPage( const std::string& file )
{
	m_currentPage = file;

	// TODO
}


std::string CGUIManager::GetCurrentPage() const
{
	return m_currentPage;
}


void CGUIManager::SetCurrentPage( const std::string& page )
{
	DEBUG_ASSERT( m_resources.find( page ) != m_resources.end() );

	m_currentPage = page;
}


void CGUIManager::Update( float deltaTime )
{
	get( m_currentPage )->Update( deltaTime );
}

void CGUIManager::Render()
{
	m_renderer->BeginRender();
	get( m_currentPage )->Render(m_renderer);
	m_renderer->EndRender();
}

