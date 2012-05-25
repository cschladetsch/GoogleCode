// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_DEREF_H_INCLUDED
#	define CGL_DEREF_H_INCLUDED

template <class T>
T &Deref(Object const &object)
{
	return *Pointer<T>(object);
}

#endif // CGL_DEREF_H_INCLUDED

//EOF


