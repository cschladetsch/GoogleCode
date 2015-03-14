// (C) 2009 christian.schladetsch@gmail.com

#ifndef TRAITS_H_INCLUDED
#define TRAITS_H_INCLUDED

CGL_BEGIN

/// type traits are used to map information to a C++ type.
template <class T>
struct Traits;

struct Null 
{ 
	virtual ~Null() { }
};

struct TypeProperties
{
	enum
	{
		Reflected = 1,
		Abstract = 2,
	};
};

/// common for all traits types
template <class T, int N, class P = Null, int Props = 0>
struct TraitsBase
{
	typedef T Type;
	typedef P ParentType;
	typedef T *PointerType;
	typedef T &ReferenceType;
	typedef T StorageType;

	enum { Number = N };
	enum { Abstract = (Props & TypeProperties::Abstract) != 0 };
	enum { Reflected = (Props & TypeProperties::Reflected) != 0 };
};

struct CGL_TypeNumbers
{
	enum Numbers
	{
		None,
		Base,
		Void,
		Int,
		Float,
		String,

		Font,
		ObjectBase,
		PhaseBase,
		Game,
		World,
	};
};

/// declare the traits, given a type T and type-number N
#	define CGL_TRAITS(T, N) \
		template <> \
		struct Traits<T> : TraitsBase<T, N> { };

#	define CGL_TRAITS_DERIVED(T, N, P) \
		template <> \
		struct Traits<T> : TraitsBase<T, N, P> { };

#	define CGL_TRAITS_ABSTRACT(T, N) \
		template <> \
		struct Traits<T> : TraitsBase<T, N, ::Null, TypeProperties::Abstract> { };

#	define CGL_TRAITS_DERIVED_ABSTRACT(T, N, P) \
		template <> \
		struct Traits<T> : TraitsBase<T, N, P, TypeProperties::Abstract> { };

#	define CGL_TRAITS_REFLECTED(T, N, P) \
		template <> \
		struct Traits<T> : TraitsBase<T, N, ::Null, TypeProperties::Reflected> { };

#	define CGL_TRAITS_DERIVED_REFLECTED(T, N, P) \
		template <> \
		struct Traits<T> : TraitsBase<T, N, P, TypeProperties::Reflected> { };

CGL_TRAITS(Null, CGL_TypeNumbers::None);

/// the first 100 type-numbers are reserved by the CGL library
#	define CGL_TYPE_NUMBER_LAST 100

CGL_END

#endif // TRAITS_H_INCLUDED

//EOF

