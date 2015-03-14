// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_COLORVALUE_H_INCLUDED
#define CGL_COLORVALUE_H_INCLUDED

CGL_BEGIN

typedef Unsigned16 Color;

/// normalised rgb color value
struct ColorRGB
{
	float red, green, blue;

	ColorRGB(float R = 1, float G = 1, float B = 1) : red(R), green(G), blue(B) { }
};

CGL_END

#endif // CGL_COLORVALUE_H_INCLUDED

//EOF
