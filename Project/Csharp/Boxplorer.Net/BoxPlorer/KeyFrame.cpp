#include "Precompiled.h"

#define NO_SDL_GLEXT

#include "KeyFrame.h"
#include "Global.h"
#include <SDL_opengl.h>

#include "GL/glext.h"

#pragma warning (disable:4244)
#pragma warning (disable:4996)
#pragma warning (disable:4305)


// CJS TODO merge with zNear/zFar defines in boxplorer.cc
#define zNear 0.0001f
#define zFar  5.0f

KeyFrame::KeyFrame() 
{ 
	memset(this, 0, sizeof *this); 
}

float KeyFrame::distanceTo(const KeyFrame& other) const 
{
	float delta[3] =  
	{ 
		v[12] - other.v[12],
		v[13] - other.v[13],
		v[14] - other.v[14] 
	};
	return sqrt(dot(delta, delta));
}

void KeyFrame::orthogonalize() 
{
	if (!normalize(ahead())) 
	{ 
		ahead()[0] = ahead()[1] = 0; 
		ahead()[2] = 1; 
	}

	float l = dot(ahead(), up());

	for (int i=0; i<3; i++) 
	{
		up()[i] -= l*ahead()[i];
	}

	if (!normalize(up())) 
	{  
		// Error? Make upDirection.z = 0.
		up()[2] = 0;
		if (fabs(ahead()[2]) == 1) 
		{
			up()[0] = 0;
			up()[1] = 1;
		} 
		else 
		{
			up()[0] = -ahead()[1];
			up()[1] = ahead()[0];
			normalize(up());
		}
	}

	// Compute rightDirection as a cross product of upDirection and direction.
	for (int i=0; i<3; i++) 
	{
		int j = (i+1)%3, k = (i+2)%3;
		right()[i] = up()[j]*ahead()[k] - up()[k]*ahead()[j];
	}
}

// Move camera in a direction relative to the view direction.
// Behaves like `glTranslate`.
void KeyFrame::move(float x, float y, float z) 
{
	for (int i=0; i<3; i++) 
	{
		pos()[i] += right()[i]*x + up()[i]*y + ahead()[i]*z;
	}
}

// Move camera in the normalized absolute direction `dir` by `len` units.
void KeyFrame::moveAbsolute(float* dir, float len) 
{
	for (int i=0; i<3; i++) 
	{
		pos()[i] += len * dir[i];
	}
}

// Rotate the camera by `deg` degrees around a normalized axis.
// Behaves like `glRotate` without normalizing the axis.
void KeyFrame::rotate(float deg, float x, float y, float z) 
{
	float s = sin(deg*PI/180), c = cos(deg*PI/180), t = 1-c;
	float r[3][3] = 
	{
		{ x*x*t +   c, x*y*t + z*s, x*z*t - y*s },
		{ y*x*t - z*s, y*y*t +   c, y*z*t + x*s },
		{ z*x*t + y*s, z*y*t - x*s, z*z*t +   c }
	};
	for (int i=0; i<3; i++) 
	{
		float c[3];
		for (int j=0; j<3; j++) c[j] = v[i+j*4];
		for (int j=0; j<3; j++) v[i+j*4] = dot(c, r[j]);
	}
}

// Set the OpenGL modelview matrix to the camera matrix, for shader.
void KeyFrame::activate() const 
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(v);
}

// Set the OpenGL modelview and projection for gl*() functions.
void KeyFrame::activateGl() 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float fH = tan( fov_y * PI / 360.0f ) * zNear;
	float fW = tan( fov_x * PI / 360.0f ) * zNear;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);

	orthogonalize();
	float matrix[16] = 
	{
		right()[0], up()[0], -ahead()[0], 0,
		right()[1], up()[1], -ahead()[1], 0,
		right()[2], up()[2], -ahead()[2], 0,
		0,       0,           0, 1
	};
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matrix);
	glTranslatef(-pos()[0], -pos()[1], -pos()[2]);
}

// Load configuration.
bool KeyFrame::loadConfig(char const* configFile, std::string* defines) 
{
	bool result = false;
	FILE* f;
	if ((f = fopen(configFile, "r")) != 0) 
	{
		size_t i;
		char s[32768];  // max line length
		while (fscanf(f, " %s", s) == 1) 
		{  
			// read word
			if (s[0] == 0 || s[0] == '#') continue;

			int v;

			// Parse #defines out of config.cfg to prepend to .glsl
			if (defines) 
			{
				if (!strcmp(s, "d") || !strcmp(s, "c")) 
				{
					std::string a(s);
					v = fscanf(f, " %s", s);
					if (v == 1) 
					{
						std::string define = "#define " + a + " " + s + "\n";
						printf(__FUNCTION__ " : %s", define.c_str());
						defines->append(define);
					}
				}
			}

			double val;

			if (!strcmp(s, "position")) { v=fscanf(f, " %f %f %f", &pos()[0], &pos()[1], &pos()[2]); continue; }
			if (!strcmp(s, "direction")) { v=fscanf(f, " %f %f %f", &ahead()[0], &ahead()[1], &ahead()[2]); continue; }
			if (!strcmp(s, "upDirection")) { v=fscanf(f, " %f %f %f", &up()[0], &up()[1], &up()[2]); continue; }

#define PROCESS(type, name, nameString, doSpline) \
	if (!strcmp(s, nameString)) { v=fscanf(f, " %lf", &val); name = val; continue; }
				PROCESS_CONFIG_PARAMS
#undef PROCESS

			for (i=0; i<lengthof(par); i++) 
			{
				char p[256];
				sprintf(p, "par%lu", (unsigned long)i);
				if (!strcmp(s, p)) 
				{
					v=fscanf(f, " %f %f %f", &par[i][0], &par[i][1], &par[i][2]);
					break;
				}
			}
		}
		fclose(f);
		printf(__FUNCTION__ " : read '%s'\n", configFile);
		result = true;
	} 
	else 
	{
		printf(__FUNCTION__ " : failed to open '%s'\n", configFile);
	}
	
	if (result) 
		sanitizeParameters();
	
	return result;
}

// Make sure parameters are OK.
void KeyFrame::sanitizeParameters(void) 
{
	// Resolution: if only one coordinate is set, keep 4:3 aspect ratio.
	if (width < 1) 
	{
		if (height < 1) 
		{ 
			height = 480; 
		}
		width = height*4/3;
	}

	if (height < 1) 
	{
		height = width*3/4;
	}

	// FOV: keep pixels square unless stated otherwise.
	// Default FOV_y is 75 degrees.
	if (fov_x <= 0) 
	{
		if (fov_y <= 0) { fov_y = 75; }
		fov_x = atan(tan(fov_y*PI/180/2)*width/height)/PI*180*2;
	}
		
	if (fov_y <= 0) fov_y = atan(tan(fov_x*PI/180/2)*height/width)/PI*180*2;

	// Fullscreen: 0=off, anything else=on.
	if (fullscreen != 0 && fullscreen != 1) fullscreen = 1;

	// The others are easy.
	if (multisamples < 1) multisamples = 1;
	//if (speed <= 0) speed = 0.005;  // units/frame
	if (keyb_rot_speed <= 0) keyb_rot_speed = 5;  // degrees/frame
	if (mouse_rot_speed <= 0) mouse_rot_speed = 1;  // degrees/pixel
	if (max_steps < 1) max_steps = 128;
	if (min_dist <= 0) min_dist = 0.0001;
	if (iters < 1) iters = 13;
	if (color_iters < 0) color_iters = 9;
	if (ao_eps <= 0) ao_eps = 0.0005;
	if (ao_strength <= 0) ao_strength = 0.1;
	if (glow_strength <= 0) glow_strength = 0.25;
	if (dist_to_color <= 0) dist_to_color = 0.2;
	if (z_near <= 0) z_near = zNear;
	if (z_far <= 0) z_far = zFar;

	orthogonalize();

	// Don't do anything with user parameters - they must be
	// sanitized (clamped, ...) in the shader.
}

// Save configuration.
void KeyFrame::saveConfig(char const* configFile, std::string* defines) 
{
	FILE* f;
	if ((f = fopen(configFile, "w")) != 0) 
	{
		if (defines != NULL)
			fprintf(f, "%s", defines->c_str());
#define PROCESS(type, name, nameString, doSpline) \
fprintf(f, nameString " %g\n", (double)name);
		PROCESS_CONFIG_PARAMS
#undef PROCESS

		fprintf(f, "position %g %g %g\n", pos()[0], pos()[1], pos()[2]);
		fprintf(f, "direction %g %g %g\n", ahead()[0], ahead()[1], ahead()[2]);
		fprintf(f, "upDirection %g %g %g\n", up()[0], up()[1], up()[2]);
		for (size_t i=0; i<lengthof(par); i++) 
		{
			fprintf(f, "par%lu %g %g %g\n", (unsigned long)i, par[i][0], par[i][1], par[i][2]);
		}
		fclose(f);
		printf(__FUNCTION__ " : wrote '%s'\n", configFile);
	}
}

// Send parameters to gpu.
void KeyFrame::setUniforms(float x_scale, float x_offset,
	float y_scale, float y_offset,
	float speed) 
{
#define glSetUniformf(name) \
	glUniform1f(glGetUniformLocation(program, #name), name);

#define glSetUniformv(name) \
	glUniform3fv(glGetUniformLocation(program, #name), lengthof(name), (float*)name);

#define glSetUniformi(name) \
	glUniform1i(glGetUniformLocation(program, #name), name);

		glSetUniformf(fov_x); 
		glSetUniformf(fov_y);
		glSetUniformi(max_steps); 
		glSetUniformf(min_dist);
		glSetUniformi(iters); 
		glSetUniformi(color_iters);
		glSetUniformf(ao_eps); 
		glSetUniformf(ao_strength);
		glSetUniformf(glow_strength); 
		glSetUniformf(dist_to_color);
		glSetUniformf(x_scale); 
		glSetUniformf(x_offset);
		glSetUniformf(y_scale); 
		glSetUniformf(y_offset);
		glSetUniformf(speed); 
		glSetUniformi(nrays);
		glSetUniformf(time);

		glUniform1f(glGetUniformLocation(program, "xres"), width);

		glSetUniformv(par);

#undef glSetUniformf
#undef glSetUniformv
#undef glUnifor1i
}

void KeyFrame::render(enum StereoMode stereo) 
{
	float p[3] = 
	{ 
		pos()[0], pos()[1], pos()[2] 
	};
	
	switch(stereo) 
	{
	case ST_OVERUNDER:
		move(speed, 0, 0);  // step right
		activate();
		setUniforms(1.0-asymmetry/2, -asymmetry/2, 2.0, 1.0);
		glRects(-1,-1,1,0);  // draw bottom half of screen
		pos()[0] = p[0]; pos()[1] = p[1]; pos()[2] = p[2];  // restore pos
		move(-speed, 0, 0);  // step left
		activate();
		setUniforms(1.0-asymmetry/2, asymmetry/2, 2.0, -1.0);
		glRects(-1,0,1,1);  // draw top half of screen
		pos()[0] = p[0]; pos()[1] = p[1]; pos()[2] = p[2];
		break;
	case ST_XEYED:
		move(speed, 0, 0);  // step right
		activate();
		setUniforms(2.0*(1.0-asymmetry/2), 1.0-asymmetry, 1.0, 0.0);
		glRectf(-1,-1,0,1);  // draw left half of screen
		pos()[0] = p[0]; pos()[1] = p[1]; pos()[2] = p[2];
		move(-speed, 0, 0);  // step left
		activate();
		setUniforms(2.0*(1.0-asymmetry/2), -1.0+asymmetry, 1.0, 0.0);
		glRectf(0,-1,1,1);  // draw right half of screen
		pos()[0] = p[0]; pos()[1] = p[1]; pos()[2] = p[2];
		break;
	case ST_NONE:
		activate();
		setUniforms(1.0, 0.0, 1.0, 0.0);
		glRects(-1,-1,1,1);  // draw entire screen
		break;
	case ST_INTERLACED:
		activate();
		//TODO: add asymmetric frustum to interlaced rendering.
		setUniforms(1.0, 0.0, 1.0, 0.0, speed);
		glRects(-1,-1,1,1);  // draw entire screen
		break;
	}
}


void LoadKeyFrames(bool fixedFov) 
{
	char filename[256];
	for (int i = 0; ; ++i) 
	{
		sprintf(filename, "%s-%u.cfg", kKEYFRAME, i);
		KeyFrame tmp;
		if (!tmp.loadConfig(filename)) break;
		if (fixedFov) 
		{
			tmp.width = config.width;
			tmp.height = config.height;
			tmp.fov_x = config.fov_x;
			tmp.fov_y = config.fov_y;
		}
		keyframes.push_back(tmp);
	}
	printf(__FUNCTION__ " : loaded %lu keyframes\n", (unsigned long)keyframes.size());
}

void SaveKeyFrames() 
{
	char filename[256];
	for (size_t i = 0; i < keyframes.size(); ++i) 
	{
		sprintf(filename, "%s-%lu.cfg", kKEYFRAME, (unsigned long)i);
		keyframes[i].saveConfig(filename);
	}
}


//EOF
