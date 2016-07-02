#ifndef COLOR_SPACE_H_
#define COLOR_SPACE_H_

#include "Color.h"
#include "Vector4.h"

class CColorSpace : public CColor
{
public:
	explicit CColorSpace(bool hsl = false) : CColor(1.0f, 1.0f, 1.0f, 1.0f), HSL(hsl) { }
	explicit CColorSpace(const Vect4f& vect, bool hsl = false) : CColor(vect), HSL(hsl) { }
	explicit CColorSpace(float red, float green, float blue, float alpha = 1.0f, bool hsl = false) : CColor(red, green, blue, alpha), HSL(hsl) { }

	bool HSL;
	inline void ToggleColorSpace();
	inline CColor toHSL();
	inline CColor toRGB();
};

#include "ColorSpace.inl"

#endif