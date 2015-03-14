// (C) 2009 christian.schladetsch@gmail.com

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

CGL_BEGIN

/// returns a random number in range 0..1
float RandomUnit();

/// returns a random number in range min..max
float RandomRanged(float min, float max);

/// returns a unit-vector pointing in a random direction
Vector RandomUnitVector();

CGL_END

#endif // RANDOM_H_INCLUDED

//EOF
