#pragma once

#include <math.h>
#include <string>

#include "Vector3d.h"

#define NUMPARS 20

//
// A frame of reference that can be used for animation.
//
// Also used to represent the camera.
//
class KeyFrame 
{
public:
	// Configuration parameters.
// [type, variable name, config name, spline]
#define PROCESS_CONFIG_PARAMS \
	PROCESS(int, width, "width", false) \
	PROCESS(int, height, "height", false) \
	PROCESS(int, fullscreen, "fullscreen", false) \
	PROCESS(int, multisamples, "multisamples", false) \
	PROCESS(float, keyb_rot_speed, "keyb_rot_speed", false) \
	PROCESS(float, mouse_rot_speed, "mouse_rot_speed", false) \
	PROCESS(float, fov_x, "fov_x", true) \
	PROCESS(float, fov_y, "fov_y", true) \
	PROCESS(float, speed, "speed", true) \
	PROCESS(float, min_dist, "min_dist", true) \
	PROCESS(int, max_steps, "max_steps", true) \
	PROCESS(int, iters, "iters", true) \
	PROCESS(int, color_iters, "color_iters", true) \
	PROCESS(int, loop, "loop", false) \
	PROCESS(float, ao_eps, "ao_eps", true) \
	PROCESS(float, ao_strength, "ao_strength", true) \
	PROCESS(float, glow_strength, "glow_strength", true) \
	PROCESS(float, dist_to_color, "dist_to_color", true) \
	PROCESS(float, delta_time, "delta_time", false) \
	PROCESS(float, time, "time", true) \
	PROCESS(float, fps, "fps", false) \
	PROCESS(int, depth_size, "depth_size", false) \
	PROCESS(float, z_near, "z_near", true) \
	PROCESS(float, z_far, "z_far", true) \
	PROCESS(float, dof_scale, "dof_scale", true) \
	PROCESS(float, dof_offset, "dof_offset", true) \
	PROCESS(int, enable_dof, "enable_dof", false) \
	PROCESS(int, no_spline, "no_spline", false) \
	PROCESS(float, asymmetry, "asymmetry", true) \
	PROCESS(int, nrays, "nrays", true)
	
// Declare fractal and other parameters.
#define PROCESS(a,b,c,d) a b;
	PROCESS_CONFIG_PARAMS
#undef PROCESS

	// View matrix.
	float v[16];

	// Par[] parameter array.
	float par[NUMPARS][3];  // min(this, glsl) gets sent to shader.

	bool isKey_;  // Whether this frame is actually a KeyFrame.

	KeyFrame();

	float* right() { return &v[0]; }
	float* up() { return &v[4]; }
	float* ahead() { return &v[8]; }
	float* pos() { return &v[12]; }

	void setKey(bool key) { isKey_ = key; }
	bool isKey() const { return isKey_; }

	float distanceTo(const KeyFrame& other) const;

	void orthogonalize();

	// Move camera in a direction relative to the view direction.
	// Behaves like `glTranslate`.
	void move(float x, float y, float z);

	// Move camera in the normalized absolute direction `dir` by `len` units.
	void moveAbsolute(float* dir, float len);

	// Rotate the camera by `deg` degrees around a normalized axis.
	// Behaves like `glRotate` without normalizing the axis.
	void rotate(float deg, float x, float y, float z);

	// Set the OpenGL modelview matrix to the camera matrix, for shader.
	void activate() const;

	// Set the OpenGL modelview and projection for gl*() functions.
	void activateGl();

	// Load configuration.
	bool loadConfig(char const* configFile, std::string* defines = NULL);

	// Make sure parameters are OK.
	void sanitizeParameters(void);

	// Save configuration.
	void saveConfig(char const* configFile, std::string* defines = NULL);

	// Send parameters to gpu.
	void setUniforms(float x_scale, float x_offset,
		float y_scale, float y_offset,
		float speed = 0.0);

	void render(enum StereoMode stereo);
};

//EOF
