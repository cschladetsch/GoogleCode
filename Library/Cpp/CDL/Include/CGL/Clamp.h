// (C) 2009 christian.schladetsch@gmail.com

#ifndef CLAMP_H_INCLUDED
#	define CLAMP_H_INCLUDED

CGL_BEGIN

/// returns a value clamped between a min and max limit
template <class Scalar>
Scalar Clamp(Scalar val, Scalar min, Scalar max)
{
	return std::max(std::min(max, val), min);
}

CGL_END

#endif // CLAMP_H_INCLUDED

//EOF
