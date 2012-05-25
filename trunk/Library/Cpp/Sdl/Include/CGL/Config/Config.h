// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_CONFIG_H
#	define CGL_CONFIG_H

//#	define CGL_CONFIG_USE_NAMESPACES
#	ifdef CGL_CONFIG_USE_NAMESPACES
#		define CGL_NAMESPACE_NAME cgl
#		define CGL_BEGIN namespace CGL_NAMESPACE_NAME {
#		define CGL_END }
#	else
#		define CGL_BEGIN 
#		define CGL_END
#	endif // CGL_CONFIG_USE_NAMESPACES
#endif // CGL_CONFIG_H

//EOF

