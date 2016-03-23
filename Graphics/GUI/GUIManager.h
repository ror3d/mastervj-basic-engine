#pragma once

#include <Base/Utils/TMapManager.h>

class CGUIContainer;

class CGUIManager : public TMapManager<CGUIContainer>
{
public:
	void LoadContainer( const std::string& file );
};
