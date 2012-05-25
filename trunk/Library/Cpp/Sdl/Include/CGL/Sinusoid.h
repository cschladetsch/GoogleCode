// (C) 2009 christian.schladetsch@gmail.com

#ifndef SINUSOID_H_INCLUDED
#define SINUSOID_H_INCLUDED

CGL_BEGIN

/// A sinusoidal wave
struct Sinusoid
{
	float amplitude;
	float frequency;
	float offset;
	float phase;

	Sinusoid(float A = 1, float F = 1, float O = 0, float P = 0)
		: amplitude(A), frequency(F), offset(O), phase(P)
	{
	}

	float operator()(float T) const
	{
		return offset + amplitude*sin(frequency*T + phase);
	}
};

CGL_END

#endif // SINUSOID_H_INCLUDED

//EOF
