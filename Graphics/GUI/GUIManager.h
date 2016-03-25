#pragma once

#include <Base/Utils/TMapManager.h>

class CGUIContainer;
class CGUIRenderer;

class CGUIManager : public TMapManager<CGUIContainer>
{
	std::string m_currentPage;

	CGUIRenderer *m_renderer;
public:
	CGUIManager();
	~CGUIManager();

	void LoadPage( const std::string& file );

	std::string GetCurrentPage() const;

	void SetCurrentPage( const std::string& page );

	void Update( float deltaTime );

	void Render();
};
