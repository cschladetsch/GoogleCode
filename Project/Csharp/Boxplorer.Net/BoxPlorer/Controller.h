#pragma once

typedef enum EController
{
	// user parameters: 0..9
	// other parameters:
	CTL_FOV = lengthof(camera.par), 
	CTL_RAY, 
	CTL_ITER, 
	CTL_AO, 
	CTL_GLOW,
	CTL_TIME, 
	CTL_CAM, 
	CTL_3D,
	CTL_LAST = CTL_3D,
};

extern EController Controller;

char* printController(char* s, EController c);
void updateControllerY(EController c, int d);
void updateControllerX(EController c, int d);
void changeController(SDLKey key, EController* c);

// alters the global camera
void m_rotateX2(float d);
void m_rotateY2(float d);
void m_rotateZ2(float d);

//EOF
