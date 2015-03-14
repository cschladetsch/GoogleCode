// (C) 2009 christian.schladetsch@gmail.com

/// @file Common includes for all source files

#ifndef CGL_PRECOMPILED_H
#	define CGL_PRECOMPILED_H

#	include <cstdlib>
#	include <math.h>
#	include <iostream>
#	include <assert.h>

#	include <algorithm>
#	include <list>
#	include <vector>
#	include <set>
#	include <map>
#	include <string>

#	include <SDL.h>
#	include <SDL_audio.h>

#	include <boost/foreach.hpp>

#	define foreach BOOST_FOREACH

///// #include "Platform/Config.h"
#define CGL_PLATFORM_VISTA

#ifdef CGL_PLATFORM_VISTA

#define CGL_COMPILER_NAME VisualC
#define CGL_COMPILER_VISUAL_C

#define CGL_PLATFORM_NAME Vista

#define CGL_TARGET_NAME Vista
#define CGL_TARGET_VISTA

#endif


#ifdef CGL_COMPILER_VISUAL_C
#	pragma warning(disable:4244)
#endif

//// #endif PLATFORM_CONFIG_H


typedef unsigned char Unsigned8;
typedef unsigned short Unsigned16;

typedef signed char Signed8;
typedef signed short Signed16;

typedef int TypeNumber, Handle;

typedef std::string String;

#	include "CGL/Config/Config.h"
#	include "CGL/Clamp.h"
#	include "CGL/ColorValue.h"
#	include "CGL/Geometry.h"
#	include "CGL/Matrix.h"
#	include "CGL/Direction.h"
#	include "CGL/GameTime.h"
#	include "CGL/Sinusoid.h"
#	include "CGL/Random.h"
#	include "CGL/Traits.h"

CGL_BEGIN

struct Object;
struct ObjectBase;
struct ClassBase;
struct Object;
struct Factory;

CGL_END

#	include "CGL/Event.h"
#	include "CGL/ObjectBase.h"
#	include "CGL/Storage.h"
#	include "CGL/Reflected.h"
#	include "CGL/Class.h"
#	include "CGL/Dereference.h"
#	include "CGL/Pointer.h"
#	include "CGL/ActiveObject.h"
#	include "CGL/Object.h"
#	include "CGL/Deref.h"
#	include "CGL/Factory.h"

#	include "CGL/Font.h"
#	include "CGL/Pointer.h"
#	include "CGL/PhaseBase.h"
#	include "CGL/World.h"
#	include "CGL/Game.h"

#endif

//EOF
