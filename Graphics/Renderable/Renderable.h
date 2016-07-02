#pragma once

class CContextManager;

class IRenderable
{
public:
	virtual void Render(CContextManager* context) = 0;
};
