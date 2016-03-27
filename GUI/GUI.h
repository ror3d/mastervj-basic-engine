#pragma once

#include <Base/Math/Math.h>
#include <Base/Math/Color.h>
#include <Graphics/Mesh/VertexTypes.h>
#include <vector>

template <typename T>
struct Rect
{
	union
	{
		struct
		{
			Vector2<T> position;
		};
		struct
		{
			T x;
			T y;
		};
	};
	union
	{
		struct
		{
			Vector2<T> size;
		};
		struct
		{
			T w;
			T h;
		};
	};

	bool relative = false;

	Rect() {};
	Rect( const Rect& r ) : position( r.position ), size( r.size ), relative( r.relative ) {}
	Rect( Vector2<T> pos, Vector2<T> size, bool relativeToParentSize = false) : position( pos ), size( size ), relative(relativeToParentSize) {}
	Rect( float x, float y, float w, float h, bool relativeToParentSize = false ) : position( x, y ), size( w, h ), relative(relativeToParentSize) {}
};

using Rectf = Rect<float>;


class CMaterial;
class CContextManager;
class CRenderableVertexs;

class CGUI
{
public:
	static const int MAX_GUI_ELEMENTS = 200;

	enum class Alignment
	{
		TOP_LEFT = 0x11,
		TOP_CENTER = 0x12,
		TOP_RIGHT = 0x14,
		MIDDLE_LEFT = 0x21,
		CENTER = 0x22,
		MIDDLE_RIGHT = 0x24,
		BOTTOM_LEFT = 0x41,
		BOTTOM_CENTER = 0x42,
		BOTTOM_RIGHT = 0x44
	};
	enum class MouseButtonState
	{
		UP,
		CLICKED,
		DOWN,
		RELEASED
	};

private:
	CContextManager *m_contextManager;

	bool m_inGUI;

	Rectf m_screen;

	std::vector<GUI_VERTEX> m_guiComponents;

	CRenderableVertexs *m_guiComponentsVtxs;

	std::vector<Rectf> m_nestedPositionStack;
public:
	CGUI();
	~CGUI();

	void Init( CContextManager* cm );

	void BeginGUI();
	void EndGUI();

	void Image(const std::string& material, const Rectf& r, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT, uint32 sprite = 0);

	MouseButtonState Button( const std::string& material, const Rectf& r, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT, uint32 idleSprite = 0, int32 overSprite = -1, int32 downSprite = -1);
	MouseButtonState Button( const std::string& material, const Rectf& image, const Vect2f& activeAreaSizeOffset, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT, uint32 idleSprite = 0, int32 overSprite = -1, int32 downSprite = -1);

	void BeginFrame( const Rectf& r, Alignment alignToParent = Alignment::TOP_LEFT, Alignment alignSelf = Alignment::TOP_LEFT );
	void EndFrame();

private:

	void ImageInternal( const std::string& material, const Rectf& r, uint32 sprite = 0 );

	Rectf getAligned( const Rectf &r, Alignment alignToParent, Alignment alignSelf );
	Rectf getNormalized( const Rectf &r );

	MouseButtonState getMouseState( const Rectf& bounds );
};
