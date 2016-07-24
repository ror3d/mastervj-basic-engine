#include "MathUtils.h"
#include <Windows.h>
#include "ColorSpace.h"

inline CColor CColorSpace::toHSL()
{
	if (HSL)
	{
		return CColor(x, y, z, w);
	}

	Vect3f rgb;

	float H;
	float S;
	float L;

	float cmax = max(x, max(y, z));
	float cmin = min(x, min(y, z));

	float delta = cmax - cmin;

	if (delta == 0)
	{
		H = 0;
	}
	else if (cmax == x)
	{
		float val = y - z < 0 ? 6 : 0;
		H = fmodf(((y - z) / delta), 6) + val;
	}
	else if (cmax == y)
	{
		H = (z - x) / delta + 2;
	}
	else if (cmax == z)
	{
		H = (x - y) / delta + 4;
	}

	L = (cmax + cmin) / 2;

	if (delta == 0)
	{
		S = 0;
	}
	else
	{
		S = delta / (1 - abs(2 * L - 1));
	}

	return CColor(H * 60 / 360, S, L, 1.0f);
}

inline CColor CColorSpace::toRGB()
{
	if (!HSL)
	{
		return CColor(x, y, z, w);
	}

	float H = x * 360;
	float S = y;
	float L = z;

	Vect3f rgb;

	float c = (1 - abs(2 * L - 1)) * S;
	float x = c * (1 - abs(fmodf(H / 60, 2) - 1));
	float m = L - c / 2;

	if (H < 60)
	{
		rgb.x = c;
		rgb.y = x;
		rgb.z = 0;
	}
	else if (H < 120)
	{
		rgb.x = x;
		rgb.y = c;
		rgb.z = 0;
	}
	else if (H < 180)
	{
		rgb.x = 0;
		rgb.y = c;
		rgb.z = x;
	}
	else if (H < 240)
	{
		rgb.x = 0;
		rgb.y = x;
		rgb.z = c;
	}
	else if (H < 300)
	{
		rgb.x = x;
		rgb.y = 0;
		rgb.z = c;
	}
	else if (H < 360)
	{
		rgb.x = c;
		rgb.y = 0;
		rgb.z = x;
	}

	CColor r;
	r.x = rgb.x + m;
	r.y = rgb.y + m;
	r.z = rgb.z + m;
	r.w = 1.0f;

	return r;
}

inline void CColorSpace::ToggleColorSpace()
{
	CColor c;

	if (HSL)
	{
		c = toRGB();
	}
	else
	{
		c = toHSL();
	}

	x = c.x;
	y = c.y;
	z = c.z;
	w = c.w;

	HSL = !HSL;
}