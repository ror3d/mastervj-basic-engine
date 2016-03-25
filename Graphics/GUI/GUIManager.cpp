#include "GUIManager.h"
#include "GUIContainer.h"
#include "GUIRenderer.h"

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

