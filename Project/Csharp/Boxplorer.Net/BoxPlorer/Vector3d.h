#pragma once

#include <math.h>

#ifdef PI
#undef PI
#endif
#define PI          3.14159265358979324f
#define die(...)    ( fprintf(stderr, __VA_ARGS__), exit(-1), 1 )
#define lengthof(x) ( sizeof(x)/sizeof((x)[0]) )
#define sign(x)     ( (x)<0 ? -1 : 1 )

// Compute the dot product of two vectors.
inline float dot(const float x[3], const float y[3]) 
{
	return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}

// Normalize a vector. If it was zero, return 0.
inline int normalize(float x[3]) 
{
	float len = dot(x, x); 
	
	if (len == 0) 
		return 0;
	
	len = 1/sqrt(len); 
	
	x[0] *= len; 
	x[1] *= len; 
	x[2] *= len;
	
	return 1;
}

//EOF
