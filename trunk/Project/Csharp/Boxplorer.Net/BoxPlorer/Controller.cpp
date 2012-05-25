#include "Precompiled.h"
#include "Controller.h"
#include <assert.h>

// Controller modifiers.
// They get a pointer to the modified value and a signed count of consecutive changes.

void m_mul(float* x, int d) 
{ 
	*x *= pow(10, sign(d)/20.f); 
}

void m_mulSlow(float* x, int d) 
{ 
	*x *= pow(10, sign(d)/40.f); 
}

void m_tan(float* x, int d) 
{ 
	*x = atanf(tanf(*x*PI/180.f/2.f) * pow(0.1f, sign(d)/40.f) ) /PI*180.f*2.f; 
}

void m_progressiveInc(int* x, int d) 
{ 
	*x += sign(d) * ((abs(d)+4) / 4); 
}

void m_progressiveAdd(float* x, int d) 
{ 
	*x += 0.001f * (sign(d) * ((abs(d)+4) / 4)); 
}

void m_singlePress(int* x, int d) 
{ 
	if (d==1 || d==-1) 
		*x += d; 
}

void m_rotateX(int d) { camera.rotate(sign(d)*camera.keyb_rot_speed, 0, 1, 0); }
void m_rotateY(int d) { camera.rotate(-sign(d)*camera.keyb_rot_speed, 1, 0, 0); }

void m_rotateX2(float d) { camera.rotate(d, 0, 1, 0); }
void m_rotateY2(float d) { camera.rotate(d, 1, 0, 0);}
void m_rotateZ2(float d) { camera.rotate(d, 0, 0, 1); }

// Print controller values into a string.
char* printController(char* s, EController c) 
{
	assert(c <= CTL_LAST);
	switch (c) 
	{
	case CTL_FOV: sprintf(s, "Fov %.3g %.3g", camera.fov_x, camera.fov_y); break;
	case CTL_RAY: sprintf(s, "Ray %.2e steps %d", camera.min_dist, camera.max_steps); break;
	case CTL_ITER: sprintf(s, "It %d|%d", camera.iters, camera.color_iters); break;
	case CTL_AO: sprintf(s, "aO %.2e d %.2e", camera.ao_strength, camera.ao_eps); break;
	case CTL_GLOW: sprintf(s, "Glow %.3f bgd %.2e", camera.glow_strength, camera.dist_to_color); break;
	
	case CTL_CAM: 
		{
			sprintf(s, "Look [%4d %4d %4d]",
				(int)(camera.ahead()[0]*100),
				(int)(camera.ahead()[1]*100),
				(int)(camera.ahead()[2]*100));
		} 
		break;
	
	case CTL_TIME: 
		{
			sprintf(s, "Speed %f DeltaT %.3f", camera.speed, camera.delta_time);
		} 
		break;
	
	case CTL_3D: 
		{
			sprintf(s, "Sep %f Asym %.3f", camera.speed, camera.asymmetry);
		} 
		break;
	
	default: 
		{
			char x[8],y[8]; sprintf(x, "par%dx", c); sprintf(y, "par%dy", c);
			sprintf(s, "%s %.3f %s %.3f",
				parName[c][1] ? parName[c][1] : y, camera.par[c][1],
				parName[c][0] ? parName[c][0] : x, camera.par[c][0]
			);
		} break;
	}
	return s;
}

// Update controller.y by the signed count of consecutive changes.
void updateControllerY(EController c, int d) 
{
	assert(c <= CTL_LAST);
	switch (c) 
	{
		case CTL_FOV: m_tan(&camera.fov_x, d); m_tan(&camera.fov_y, d); break;
		case CTL_RAY: m_mul(&camera.min_dist, d); break;
		case CTL_ITER: m_singlePress(&camera.iters, d); if ((camera.iters&1)==(d>0)) m_singlePress(&camera.color_iters, d); break;
		case CTL_AO: m_mulSlow(&camera.ao_strength, d); break;
		case CTL_GLOW: m_progressiveAdd(&camera.glow_strength, d); break;
		case CTL_CAM: m_rotateY(d); break;
		case CTL_TIME: m_progressiveAdd(&camera.delta_time, d); break;
		case CTL_3D: m_progressiveAdd(&camera.asymmetry, d); break;
		default: m_progressiveAdd(&camera.par[c][1], d); break;
	}

	// Enforce sane bounds.
	if (camera.delta_time < 0) camera.delta_time = 0;
	if (camera.asymmetry < 0) camera.asymmetry = 0;
}

// Update controller.x by the signed count of consecutive changes.
void updateControllerX(EController c, int d) 
{
	assert(c <= CTL_LAST);
	switch (c) 
	{
		case CTL_FOV: m_tan(&camera.fov_x, d); break;
		case CTL_RAY: m_progressiveInc(&camera.max_steps, d); break;
		case CTL_ITER: m_singlePress(&camera.color_iters, d); break;
		case CTL_AO: m_mul(&camera.ao_eps, d); break;
		case CTL_GLOW: m_mul(&camera.dist_to_color, d); break;
		case CTL_CAM: m_rotateX(d); break;
		case CTL_TIME: m_mulSlow(&camera.speed, d); break;
		case CTL_3D: m_mulSlow(&camera.speed, d); break;
		default: m_progressiveAdd(&camera.par[c][0], d); break;
	}
}

// Change the active controller with a keypress.
void changeController(SDLKey key, EController* c) 
{
	switch (key) 
	{
		case SDLK_0: case SDLK_KP0: *c = (EController)0; break;
		case SDLK_1: case SDLK_KP1: *c = (EController)1; break;
		case SDLK_2: case SDLK_KP2: *c = (EController)2; break;
		case SDLK_3: case SDLK_KP3: *c = (EController)3; break;
		case SDLK_4: case SDLK_KP4: *c = (EController)4; break;
		case SDLK_5: case SDLK_KP5: *c = (EController)5; break;
		case SDLK_6: case SDLK_KP6: *c = (EController)6; break;
		case SDLK_7: case SDLK_KP7: *c = (EController)7; break;
		case SDLK_8: case SDLK_KP8: *c = (EController)8; break;
		case SDLK_9: case SDLK_KP9: *c = (EController)9; break;
		case SDLK_f: *c = CTL_FOV; break;
		case SDLK_g: *c = CTL_GLOW; break;
		case SDLK_l: *c = CTL_CAM; break;
		case SDLK_i: *c = CTL_ITER; break;
		case SDLK_o: *c = CTL_AO; break;
		case SDLK_r: *c = CTL_RAY; break;
		case SDLK_t: *c = CTL_TIME; break;
		case SDLK_v: *c = CTL_3D; break;
		default: break;  // no change
	}
}

//EOF
