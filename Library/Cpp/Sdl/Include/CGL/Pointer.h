// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_POINTER_H
#	define CGL_POINTER_H

/// a strongly-typed object
template <class T>
struct Pointer : Object
{
	typedef Traits<T> MyTraits;
	typedef typename MyTraits::PointerType PointerType;
	typedef typename MyTraits::ReferenceType ReferenceType;

	Pointer()
	{
	}

	Pointer(Object const &base)
	{
		*this = &base;
	}
	Pointer(Object const *base)
	{
		*this = base;
	}

	Pointer<T> &operator=(Object const &base)
	{
		return *this = &base;
	}
	Pointer<T> &operator=(Object const *base)
	{
		if (!base)
		{
			Object::operator=(Object());
			return *this;
		}
		bool can_assign = !base->Exists() || base->IsType<T>() || base->IsConvertible<T>();
		assert(can_assign);
		if (can_assign)
			Object::operator=(*base);
		return *this;
	}

	PointerType operator->()
	{
		assert(Exists());
		return GetPointer();
	}

	ReferenceType operator*()
	{
		assert(Exists());
		return *GetPointer();
	}
	
	PointerType GetPointer() const
	{
		Object *base = Self();
		if (!base)
			return 0;
		if (!base->IsConvertible<T>())
			return 0;
		return Dereference<T>::From(base);
	}
};

template <class T>
struct BasePointer : Object
{
};

#endif // CGL_POINTER_H

//EOF
