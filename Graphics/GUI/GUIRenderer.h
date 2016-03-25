#pragma once

#include <Base/Math/Vector2.h>
#include <Base/Math/Color.h>

class CMaterial;

class CGUIRenderer
{
public:
	struct RenderSurface
	{
		Vect2f position;
		Vect2f size;
		struct
		{
			Vect2f tl;
			Vect2f br;
		} uv;
		CColor color;
	};

	void Render(RenderSurface surface, CMaterial *material);

	void BeginRender();
	void EndRender();
};
