// (C) 2009 christian.schladetsch@gmail.com

#ifndef CLASS_H_INCLUDED
#define CLASS_H_INCLUDED

CGL_BEGIN

/// common for all Class<T> structures
struct ClassBase
{
protected:
	TypeNumber type_number;
	Factory *factory;

public:
	ClassBase(TypeNumber N, Factory *F) : type_number(N), factory(F) { }

	TypeNumber GetTypeNumber() const { return type_number; }
	Factory *GetFactory() const { return factory; }

	virtual Object *Create(Handle) const = 0;
	virtual TypeNumber GetParentTypeNumber() const = 0;
};

/// a class for a given type T; used to make new instances
template <class T>
struct Class : ClassBase
{
	Class(Factory *F) : ClassBase(Traits<T>::Number, F) { }

	Object *Create(Handle handle) const
	{
		Storage<T> *ptr = new Storage<T>(this, handle);
		AddReflection<Traits<T>::Reflected>::Create(ptr);
		return ptr;
	}
	TypeNumber GetParentTypeNumber() const
	{
		return Traits<typename Traits<T>::ParentType>::Number;
	}
};

CGL_END

#endif // CLASS_H_INCLUDED

//EOF
