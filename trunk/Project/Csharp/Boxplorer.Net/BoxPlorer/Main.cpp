#include "Precompiled.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>

#include "GlShaderFunctions.h"
#include "DefaultShaders.h"
#include "TGA.h"
#include "CatmullRom.h"
#include "Utility.h"
#include "WoxPlorer.h"

#pragma warning(disable: 4996) // unsafe function
#pragma warning(disable: 4244) // conversion loss
#pragma warning(disable: 4305) // truncation
#pragma warning(disable: 4800) // forcing value to bool

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

using namespace std;

///
/// the entry point for the renderer
///
int woxmain(int argc, const char **argv) 
{
	bool rendering = false;
	bool loop = false;
	bool useTime = false;
	bool configSpeed = false;
	bool fixedFov = false;
	int enableDof = 0;

	// Peel known options off the back..
	while (argc > 1) 
	{
		if (!strcmp(argv[argc-1], "--overunder")) {
			stereoMode = ST_OVERUNDER;
		} else if (!strcmp(argv[argc-1], "--interlaced")) {
			stereoMode = ST_INTERLACED;
		} else if (!strcmp(argv[argc-1], "--xeyed")) {
			stereoMode = ST_XEYED;
		} else if (!strcmp(argv[argc-1], "--render")) {
			rendering = true;
		} else if (!strcmp(argv[argc-1], "--time")) {
			useTime = true;
		} else if (!strcmp(argv[argc-1], "--speed")) {
			configSpeed = true;
		} else if (!strcmp(argv[argc-1], "--disabledof")) {
			enableDof = -1;
		} else if (!strcmp(argv[argc-1], "--enabledof")) {
			enableDof = 1;
		} else if (!strcmp(argv[argc-1], "--fixedfov")) {
			fixedFov = true;
		} else if (!strcmp(argv[argc-1], "--loop")) {
			loop = true;
		} else if (!strncmp(argv[argc-1], "--kf=", 5)) {
			kKEYFRAME = argv[argc-1] + 5;
		} else break;
		--argc;
	}

	const char* configFile = (argc>=2 ? argv[1] : DEFAULT_CONFIG_FILE);

	// Load configuration.
	if (config.loadConfig(configFile, &defines)) 
	{
		changeWorkingDirectory(configFile);
	}
	else 
	{
		die("Usage: boxplorer <configuration-file.cfg>");
	}

	// Sanitize / override config parameters.
	if (loop) config.loop = true;
	if (enableDof) config.enable_dof = (enableDof == 1);  // override
	if (config.fps < 5) config.fps = 30;
	if (config.depth_size < 16) config.depth_size = 16;
	if (stereoMode == ST_XEYED) config.width *= 2;

	camera = config;

	bool keyframesChanged = false;
	LoadKeyFrames(fixedFov);
	LoadBackground();

	// Initialize SDL and OpenGL graphics.
	SDL_Init(SDL_INIT_VIDEO) == 0 ||
		die("SDL initialization failed: %s\n", SDL_GetError());
	atexit(SDL_Quit);

	// Set up the video mode, OpenGL state, shaders and shader parameters.
	initGraphics();
#ifdef USE_TW
	initTwBar();
#endif
	initFPS(FPS_FRAMES_TO_AVERAGE);

	// Main loop.
	EController ctl = CTL_CAM;  // the default controller is camera rotation
	int consecutiveChanges = 0;

	int done = 0;
	int frameno = 0;

	std::vector<KeyFrame> splines;
	std::size_t splines_index = 0;

	float frame_time = 1 / config.fps;
	float render_time = 0;
	float render_start = 0;

	float dist_along_spline = 0;
	size_t keyframe = keyframes.size();

	bool ignoreNextMouseUp = false;
	bool dragging = false;

	if (rendering) {
		// Rendering a sequence to disk. Spline the keyframes.
		CatmullRom(keyframes, &splines, config.loop);
	}

	while (!done) {
		int ctlXChanged = 0, ctlYChanged = 0;

		// Splined keyframes playback logic. Mess.
		if (!splines.empty()) {
			if (rendering && splines_index >= splines.size()) break;  // done

			// Loop if asked to.
			if (config.loop && splines_index >= splines.size()) {
				splines_index = 0;
				render_time = 0;
				render_start = now();
			}

			// Figure out whether to draw a splined frame or skip to next.
			if (splines_index < splines.size()) {
				if (!config.loop && splines_index == splines.size() - 1) {
					camera = keyframes[keyframes.size() - 1];  // End at last position.
				} else {
					camera = splines[splines_index];
				}
				if (splines_index > 0) {
					dist_along_spline += camera.distanceTo(splines[splines_index - 1]);
				}
				size_t prev_splines_index = splines_index;
				++splines_index;
				if (useTime) {
					if (rendering) {
						// Rendering sequece, use desired fps timing.
						if (camera.time < render_time) continue;
						render_time += frame_time;
					} else {
						// Previewing. Use real time (low framerate == jumpy preview!).
						float n = now();
						if (n > render_start + camera.time) continue;  // late, skip frame.
						float w = (render_start + camera.time) - n;
						if (w >= frame_time) {  // early, redraw frame.
							splines_index = prev_splines_index;
						}
					}
				} else {
					if (dist_along_spline < (configSpeed?config.speed:camera.speed))
						continue;
				}
				dist_along_spline -= (configSpeed?config.speed:camera.speed);
			} else {
				splines.clear();
			}
		}

		if (config.enable_dof && camera.dof_scale > .0001) {
			// If we have some DoF to render, render to texture.
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		}

		if (!rendering) {
			// If we're rendering a sequence to disk, we don't care about z-buffer.
			// Otherwise, just overwrite since we write every pixel.
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_ALWAYS);
		}

		if (background_texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, background_texture);
			glUseProgram(program);
			glUniform1i(glGetUniformLocation(program, "bg_texture"), 0);
		} else {
			glUseProgram(program);
		}
		glUniform1i(glGetUniformLocation(program, "use_bg_texture"), background_texture);

		camera.render(stereoMode);

		glUseProgram(0);

		if (background_texture) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (!config.no_spline && stereoMode == ST_NONE && keyframes.size() > 1 && splines.empty()) 
		{
			// Draw keyframe splined path, if we have 2+ keyframes and not rendering
			glDepthFunc(GL_LESS);

			camera.activateGl();

			std::vector<KeyFrame> splines;
			CatmullRom(keyframes, &splines, config.loop);

			glColor4f(.8,.8,.8,1);
			glLineWidth(1);
			glEnable(GL_LINE_SMOOTH);

			glBegin(config.loop?GL_LINE_LOOP:GL_LINE_STRIP);  // right eye
			for (size_t i = 0; i < splines.size(); ++i) 
			{
				splines[i].move(splines[i].speed, 0, 0);
				glVertex3fv(splines[i].pos());
				splines[i].move(-2*splines[i].speed, 0, 0);
			}
			glEnd();
			glBegin(config.loop?GL_LINE_LOOP:GL_LINE_STRIP);  // left eye
			for (size_t i = 0; i < splines.size(); ++i) 
			{
				glVertex3fv(splines[i].pos());
			}
			glEnd();

			glLineWidth(13);
			for (size_t i = 0; i < keyframes.size(); ++i) {
				glColor4f(0,0,1 - (i/256.0),1);  // Encode keyframe # in color.
				glBegin(GL_LINES);
				KeyFrame tmp = keyframes[i];
				tmp.move(tmp.speed, 0, 0);
				glVertex3fv(tmp.pos());
				tmp = keyframes[i];
				tmp.move(-tmp.speed, 0, 0);
				glVertex3fv(tmp.pos());
				glEnd();
			}
		}

		glDisable(GL_DEPTH_TEST);

		if (config.enable_dof && camera.dof_scale > .0001) {
			// If we're rendering some DoF, draw texture on screen.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glGenerateMipmap(GL_TEXTURE_2D);

			glUseProgram(dof_program);  // Activate our alpha channel shader.

			glUniform1i(glGetUniformLocation(dof_program, "my_texture"), 0);
			glUniform1f(glGetUniformLocation(dof_program, "z_near"), camera.z_near);
			glUniform1f(glGetUniformLocation(dof_program, "z_far"), camera.z_far);
			glUniform1f(glGetUniformLocation(dof_program, "dof_scale"),
				camera.dof_scale);
			glUniform1f(glGetUniformLocation(dof_program, "dof_offset"),
				camera.dof_offset);

			// Ortho projection, entire screen in regular pixel coordinates.
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, config.width, config.height, 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// Draw our texture covering entire screen, running the frame shader.
			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
			glTexCoord2f(0,1);
			glVertex2f(0,0);
			glTexCoord2f(0,0);
			glVertex2f(0,config.height);
			glTexCoord2f(1,0);
			glVertex2f(config.width,config.height);
			glTexCoord2f(1,1);
			glVertex2f(config.width,0);
			glEnd();

			glUseProgram(0);

			glDisable(GL_TEXTURE_2D);
		}

#ifdef USE_TW
		// Draw AntTweakBar
		if (!grabbedInput && !rendering) TwDraw();
#endif

		{
			GLenum err = glGetError();
			if (err != GL_NO_ERROR) printf("glGetError():%04x\n", err);
		}

		SDL_GL_SwapBuffers();

		float dist = distanceToSurface();

		if (rendering && !splines.empty()) 
		{
			// If we're playing a sequence back, save every frame to disk.
			char filename[256];
			sprintf(filename, "frame-%05d.tga", frameno);
			++frameno;
			saveScreenshot(filename);
		}

		updateFPS();

		// Show position and fps in the caption.
		char caption[2048], controllerStr[256];
		sprintf(caption, "%s %.2ffps %5lu [%.3f %.3f %.3f] %dms",
			printController(controllerStr, ctl),
			getFPS(), (unsigned long)splines_index,
			camera.pos()[0], camera.pos()[1], camera.pos()[2],
			getLastFrameDuration()
			);
		SDL_WM_SetCaption(caption, 0);

		// Process events.
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			if (grabbedInput 
#ifdef USE_TW
				|| !TwEventSDL(&event)
#endif
				) 
				switch (event.type) 
			{
			case SDL_VIDEORESIZE: {
				config.width = event.resize.w;
				config.height = event.resize.h;
				config.fov_x = 0;  // go for square pixels..
				config.sanitizeParameters();
				grabbedInput = 1; 
				initGraphics(); 
#ifdef USE_TW
				initTwBar();
#endif
				printf("resize(%d, %d)\n", config.width, config.height);
				} break;

			case SDL_QUIT: done |= 1; break;

			case SDL_MOUSEBUTTONDOWN: 
				{
					if (grabbedInput == 0) 
					{
						unsigned int bgr = getBGRpixel(event.button.x, event.button.y);
						if ((bgr & 0xffff) == 0) 
						{
							// No red or green at all : probably a keyframe marker (fragile).
							size_t kf = 255 - (bgr >> 16);
							if (kf < keyframes.size()) {
								printf("selected keyframe %lu\n", (unsigned long)kf);
								keyframe = kf;
								dragging = true;
							}
						}
					} 
					else
						switch(event.button.button) 
					{
				case 1:  // left mouse
					grabbedInput = 0;
					SDL_ShowCursor(SDL_ENABLE);
					SDL_WM_GrabInput(SDL_GRAB_OFF);
					ignoreNextMouseUp = true;
					break;
				case 4:  // mouse wheel up, increase speed at keyframe
					if (keyframe < keyframes.size()) {
						keyframes[keyframe].speed *= 1.1;
					}
					break;
				case 5:  // mouse wheel down, decrease speed at keyframe
					if (keyframe < keyframes.size()) {
						keyframes[keyframe].speed *= .9;
					}
					break;
				}
									  } break;

			case SDL_MOUSEMOTION: {
				if (grabbedInput == 0) {
					if (!dragging) {
						unsigned int bgr = getBGRpixel(event.motion.x, event.motion.y);
						if ((bgr & 0xffff) == 0) {
							// No red or green at all : probably a keyframe marker.
							size_t kf = 255 -(bgr >> 16);
							if (kf < keyframes.size()) {
								printf("keyframe %lu\n", (unsigned long)kf);
								SDL_SetCursor(hand_cursor);
							} else {
								SDL_SetCursor(arrow_cursor);
							}
						} else {
							SDL_SetCursor(arrow_cursor);
						}
					} else {
						// Drag currently selected keyframe around.
						if (keyframe < keyframes.size()) {
							// Should really be some screenspace conversion..
							// but this works ok for now.
							keyframes[keyframe].moveAbsolute(camera.up(),
								event.motion.yrel*-.05*keyframes[keyframe].speed);
							keyframes[keyframe].moveAbsolute(camera.right(),
								event.motion.xrel*.05*keyframes[keyframe].speed);
						}
					}
				}
								  } break;

			case SDL_MOUSEBUTTONUP: {
				if (ignoreNextMouseUp == false && grabbedInput == 0) {
					grabbedInput = 1;
					SDL_SetCursor(arrow_cursor);
					SDL_ShowCursor(SDL_DISABLE);
					SDL_WM_GrabInput(SDL_GRAB_ON);
				}
				ignoreNextMouseUp = false;
				dragging = false;
									} break;

			case SDL_KEYDOWN: switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: {
				if (grabbedInput && !config.fullscreen) {
					grabbedInput = 0;
					SDL_ShowCursor(SDL_ENABLE);
					SDL_WM_GrabInput(SDL_GRAB_OFF);
				} else done |= 1;
							  } break;

				// Switch fullscreen mode (loses the whole OpenGL context in Windows).
			case SDLK_RETURN: 
			case SDLK_KP_ENTER: 
				{
					config.fullscreen ^= 1; 
					grabbedInput = 1; 
					initGraphics(); 
#ifdef USE_TW
					initTwBar();
#endif
				}
				break;

				// Save config and screenshot (filename = current time).
			case SDLK_SYSREQ:
			case SDLK_PRINT: {
				time_t t = time(0);
				struct tm* ptm = localtime(&t);
				char filename[256];
				strftime(filename, 256, "%Y%m%d_%H%M%S.cfg", ptm);
				camera.saveConfig(filename);
				strftime(filename, 256, "%Y%m%d_%H%M%S.tga", ptm);
				saveScreenshot(filename);
							 } break;

				// Splined path control.
			case SDLK_HOME: {  // Start spline playback from start.
				if (!keyframes.empty()) {
					CatmullRom(keyframes, &splines, config.loop);
					splines_index = 0;
					render_time = 0;
					render_start = now();
					keyframe = 0;
				} else camera = config;
							} break;

			case SDLK_END: {  // Stop spline playback.
				splines.clear();
				if (!keyframes.empty()) {
					keyframe = keyframes.size() - 1;
					camera = keyframes[keyframe];
				} else camera = config;
						   } break;

			case SDLK_DELETE: {
				// Drop last keyframe, reset camera to previous keyframe.
				splines.clear();
				if (!keyframes.empty()) {
					if (event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL)) {
						// delete current keyframe
						if (keyframe < keyframes.size()) {
							keyframes.erase(keyframes.begin() + keyframe);
							if (keyframe >= keyframes.size())
								keyframe = keyframes.size() - 1;
						}
					} else {
						// delete last keyframe
						keyframes.pop_back();
						keyframe = keyframes.size() - 1;

						if (keyframe < keyframes.size())
							camera = keyframes[keyframe];
					}
				}
							  } break;

			case SDLK_SPACE:
			case SDLK_INSERT: {  // Add keyframe.
				splines.clear();
				size_t index = keyframes.size();
				if (event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL)) {
					// Replace currently selected keyframe.
					index = keyframe;
				}
				if (!(event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL))) {
					// Need an estimate for delta_time for this new frame.
					suggestDeltaTime(camera, keyframes);
					keyframes.push_back(camera);  // Add keyframe at end.
				} else {
					keyframes[index] = camera;  // Overwrite current keyframe.
				}
				char filename[256];
				sprintf(filename, "%s-%lu.cfg", kKEYFRAME, (unsigned long)index);
				camera.saveConfig(filename);
							  } break;

			case SDLK_PAGEUP: {  // Jump keyframe back in spline playback.
				if (splines.empty()) {
					CatmullRom(keyframes, &splines, config.loop);
					splines_index = splines.size();
				}
				if (splines_index < NSUBFRAMES)
					splines_index = 0;
				else
					splines_index -= NSUBFRAMES;
							  } break;

			case SDLK_PAGEDOWN: { // Jump splined playback keyframe ahead.
				splines_index += NSUBFRAMES;
				if (splines_index >= splines.size()) {
					splines_index = splines.size()-1;
				}
								} break;

			case SDLK_TAB: {  // Step thru keyframes.
				if (!splines.empty()) {
					// find most recently played keyframe
					int nkeys = 0;
					for (size_t i = 0; i < splines_index; ++i)
						if (splines[i].isKey()) ++nkeys;
					keyframe = nkeys;
				}
				if (!(event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL))) ++keyframe;
				if (keyframe >= keyframes.size()) keyframe = 0;

				if (keyframe < keyframes.size() && splines.empty()) {
					// Don't jump camera ahead if we were playing, just stop in place.
					camera = keyframes[keyframe];
				}

				if (keyframe < keyframes.size()) {
					printf("at keyframe %lu, speed %f, delta_time %f\n",
						(unsigned long)keyframe, keyframes[keyframe].speed,
						keyframes[keyframe].delta_time);
				}

				if (keyframes.empty()) camera = config;  // back to start

				if (event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL)) {
					// Start playing: spline and start at keyframe.
					if (!keyframes.empty()) {
						CatmullRom(keyframes, &splines, config.loop);
						int nkeys = keyframe;
						for (splines_index = 0; splines_index < splines.size();
							++splines_index) {
								if (splines[splines_index].isKey())
									if (nkeys-- == 0) break;
						}
						render_time = splines[splines_index].time;
						render_start = now() - render_time;
					}
				} else {
					splines.clear();
				}
						   } break;

			case SDLK_BACKSPACE: {
				--keyframe;
				if (keyframe >= keyframes.size()) keyframe = keyframes.size() - 1;
				if (keyframe < keyframes.size()) {
					camera = keyframes[keyframe];
					printf("at keyframe %lu, speed %f, delta_time %f\n",
						(unsigned long)keyframe, keyframes[keyframe].speed,
						keyframes[keyframe].delta_time);
				} else camera = config;
				splines.clear();
								 } break;

			case SDLK_LSHIFT: {
				// Change movement speed.
				if (camera.speed < 1) camera.speed *= 1.1;
							  } break;

			case SDLK_LALT: {
				if (camera.speed > camera.min_dist) camera.speed *= .9;
							} break;

				// Resolve controller value changes that happened during rendering.
			case SDLK_LEFT:  ctlXChanged = 1; updateControllerX(ctl, -(consecutiveChanges=1)); break;
			case SDLK_RIGHT: ctlXChanged = 1; updateControllerX(ctl,  (consecutiveChanges=1)); break;
			case SDLK_DOWN:  ctlYChanged = 1; updateControllerY(ctl, -(consecutiveChanges=1)); break;
			case SDLK_UP:    ctlYChanged = 1; updateControllerY(ctl,  (consecutiveChanges=1)); break;

				// Current keyframe manouvering in screenspace.
			case SDLK_KP4: {
				if (keyframe < keyframes.size()) {
					keyframes[keyframe].moveAbsolute(camera.right(),
						-.5*keyframes[keyframe].speed);
				}
						   } break;
			case SDLK_KP6: {
				if (keyframe < keyframes.size()) {
					keyframes[keyframe].moveAbsolute(camera.right(),
						.5*keyframes[keyframe].speed);
				}
						   } break;
			case SDLK_KP8: {
				if (keyframe < keyframes.size()) {
					keyframes[keyframe].moveAbsolute(camera.up(),
						.5*keyframes[keyframe].speed);
				}
						   } break;
			case SDLK_KP2: {
				if (keyframe < keyframes.size()) {
					keyframes[keyframe].moveAbsolute(camera.up(),
						-.5*keyframes[keyframe].speed);
				}
						   } break;

				// Adjust keyframe speed / eye separation. Like mousewheel.
			case SDLK_KP9: {
				if (keyframe < keyframes.size()) {
					keyframes[keyframe].speed *= 1.1;
				}
						   } break;
			case SDLK_KP7: {
				if (keyframe < keyframes.size()) {
					keyframes[keyframe].speed *= .9;
				}
						   } break;

				// See whether the active controller has changed.
			default: 
				{
					EController oldCtl = ctl;
					changeController(event.key.keysym.sym, &ctl);
					if (ctl != oldCtl) 
						consecutiveChanges = 0;
				} 
				break;
								  }
							  break;
			}
		}

		if (done) break;

		// Get keyboard and mouse state.
		Uint8* keystate = SDL_GetKeyState(0);
		int mouse_dx, mouse_dy;
		Uint8 mouse_buttons = SDL_GetRelativeMouseState(&mouse_dx, &mouse_dy);
		int mouse_button_left = mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		int mouse_button_right = mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		// Continue after calling SDL_GetRelativeMouseState() so view direction
		// does not jump after closing AntTweakBar.
		if (!grabbedInput) continue;

		(void)mouse_buttons;
		(void)mouse_button_left;
		(void)mouse_button_right;

		if (keystate[SDLK_w]) camera.move(0, 0, min(camera.speed , 0.9f * dist));  //forward
		if (keystate[SDLK_s]) camera.move(0, 0, -camera.speed);  //back

		if (keystate[SDLK_a]) camera.move(-camera.speed, 0, 0);  //left
		if (keystate[SDLK_d]) camera.move( camera.speed, 0, 0);  //right

		// Mouse look.
		if (grabbedInput && (mouse_dx != 0 || mouse_dy != 0)) {
			m_rotateX2(camera.mouse_rot_speed * mouse_dx);
			m_rotateY2(camera.mouse_rot_speed * mouse_dy);
		}
		if (keystate[SDLK_q]) m_rotateZ2(camera.keyb_rot_speed);
		if (keystate[SDLK_e]) m_rotateZ2(-camera.keyb_rot_speed);

		if (keystate[SDLK_z]){ if (camera.speed > camera.min_dist) camera.speed -= camera.speed/10; printf("speed %f\n", camera.speed);}
		if (keystate[SDLK_c]){ if (camera.speed < 1.0) camera.speed += camera.speed/10; printf("speed %f\n", camera.speed);}

		// Change the value of the active controller.
		if (!ctlXChanged) {
			if (keystate[SDLK_LEFT])  { ctlXChanged = 1; updateControllerX(ctl, -++consecutiveChanges); }
			if (keystate[SDLK_RIGHT]) { ctlXChanged = 1; updateControllerX(ctl,  ++consecutiveChanges); }
		}
		if (!ctlYChanged) {
			if (keystate[SDLK_DOWN])  { ctlYChanged = 1; updateControllerY(ctl, -++consecutiveChanges); }
			if (keystate[SDLK_UP])    { ctlYChanged = 1; updateControllerY(ctl,  ++consecutiveChanges); }
		}

		if (!(ctlXChanged || ctlYChanged)) consecutiveChanges = 0;
	}

#ifdef USE_TW
	TwTerminate();
#endif

	if (!rendering && keyframesChanged) 
	{
		// TODO: ask whether to save keyframes
	}
	camera.saveConfig("last.cfg", &defines);  // Save a config file on exit, just in case.
	return 0;
}

//EOF
