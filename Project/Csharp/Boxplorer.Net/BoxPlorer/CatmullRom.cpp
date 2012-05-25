#include "Precompiled.h"
#include "CatmullRom.h"

// CJS TODO this really shouldn't be ignored
#pragma warning (disable:4244)

using namespace std;

void CatmullRom(const vector<KeyFrame>& keyframes, vector<KeyFrame>* output, bool loop, int nsubframes) 
{
	output->clear();
	if (keyframes.size() < 2) return;  // Need at least two points.

	vector<KeyFrame> controlpoints(keyframes);
	if (loop) 
	{
		// Replicate first two at end.
		controlpoints.push_back(keyframes[0]);
		if (controlpoints[controlpoints.size()-1].delta_time == 0) 
		{
			suggestDeltaTime(controlpoints[controlpoints.size() - 1], keyframes);
		}
		controlpoints.push_back(keyframes[1]);
		// Last one is p0 for spline of first keyframe.
		controlpoints.push_back(keyframes[keyframes.size()-1]);
	} 
	else 
	{
		// Replicate last one twice more.
		controlpoints.push_back(keyframes[keyframes.size()-1]);
		controlpoints.push_back(keyframes[keyframes.size()-1]);
		// Last one is p0 for spline of first keyframe.
		controlpoints.push_back(keyframes[0]);
	}

	size_t n = controlpoints.size();

	// Compute controlpoint time based on sum of delta_time up to it.
	// Note we don't spline delta_time but we do spline time.
	float time = 0;
	for (size_t i = 0; i < n - 1; ++i) 
	{
		if (i) time += controlpoints[i].delta_time;
		controlpoints[i].time = time;
	}
	// Last one's time is splined into first one. Set to 0.
	controlpoints[n - 1].time = 0;

	for (size_t i = 0; i < n - 3; ++i) 
	{
		const KeyFrame *p0 = &controlpoints[i>0?i-1:n-1];
		const KeyFrame *p1 = &controlpoints[i];
		const KeyFrame *p2 = &controlpoints[i+1];
		const KeyFrame *p3 = &controlpoints[i+2];

		for (int f = 0; f < nsubframes; ++f) 
		{
			KeyFrame tmp = config;  // Start with default values.
			tmp.setKey(f == 0);
			const float t = f * (1.f / nsubframes);

			// The CatmullRom spline function; 0 <= t <= 1
			#define SPLINE(X,p0,p1,p2,p3) \
				(X = (float)(.5f * ( (2.0f * p1 + \
				(-p0 + p2)*t + \
				(2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3)*t*t + \
				(-p0 + 3.0f*p1 - 3.0f*p2 + p3)*t*t*t))))

			// Spline position, direction.
			for (size_t j = 0; j < lengthof(tmp.v); ++j) 
			{
				SPLINE(tmp.v[j], p0->v[j], p1->v[j], p2->v[j], p3->v[j]);
			}
			
			// Spline par[] array.
			for (size_t j = 0; j < lengthof(tmp.par); ++j) 
			{
				SPLINE(tmp.par[j][0], p0->par[j][0], p1->par[j][0], p2->par[j][0], p3->par[j][0]);
				SPLINE(tmp.par[j][1], p0->par[j][1], p1->par[j][1], p2->par[j][1], p3->par[j][1]);
				SPLINE(tmp.par[j][2], p0->par[j][2], p1->par[j][2], p2->par[j][2], p3->par[j][2]);
			}
			
			// Spline all other params. Some are non-sensical.
			#define PROCESS(a,b,c,doSpline) \
				if (doSpline) SPLINE(tmp.b, p0->b, p1->b, p2->b, p3->b);
			
			PROCESS_CONFIG_PARAMS
			
			tmp.orthogonalize();
			output->push_back(tmp);
		}
	}
}

//EOF
