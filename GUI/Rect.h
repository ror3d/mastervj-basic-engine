#pragma once

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
