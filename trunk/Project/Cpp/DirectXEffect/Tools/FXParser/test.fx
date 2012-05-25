// test HLSL file
//
// (C) 2009-2010 Christian Schladetsch
//

texture spam;

sampler2D sampler = sampler_state
{
	Texture = <spam>;
	MinFoo = 12;
};

float ff;

float4x4 world_transform : POSITION; // this is a comment
float4x4 foo : r_World;

void fun2(int n, float f : TEXCOORD0) : COLOR0
{
	float y = 4;
	float4 v = float4(1,2,3);
	float3 ty = (float4)x;
	x = foo.bar;
	//y = grok[spam].foo;
	n = 3*f;
	m = mul(3, n, foo, 3.24*4+67);
	z = 4+3*foo - bar*3.213;
	return 1+2*foo;
	return bar;
}

float3 fun4(float x, out float y)
{
	y = x;
}

int num;

//EOF
