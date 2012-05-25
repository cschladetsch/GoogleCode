// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_REFLECTED_H_INCLUDED
#	define CGL_REFLECTED_H_INCLUDED

CGL_BEGIN

template <class T>
struct Reflected
{
	Storage<T> *Self;

	template <class T>
	Object New() { return Self->template New<T>(); }

	void Delete() { Self->Delete(); }
};

template <bool>
struct AddReflection
{
	template <class T>
	static void Create(T) { }
};

template <>
struct AddReflection<true>
{
	template <class T>
	static void Create(Storage<T> *self) { self->GetStorage().Self = self; }
};

#endif // CGL_REFLECTED_H_INCLUDED

//EOF
