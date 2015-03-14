// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_OBJECT_H_INCLUDED
#	define CGL_OBJECT_H_INCLUDED

CGL_BEGIN

/// an object in the system; the storage is elsewhere
struct Object
{
protected:
	friend struct Factory;
	friend struct ObjectBase;
	template <class T>
	friend struct Class;

	ClassBase const *klass;	///< the class that made this object.
	Handle handle;

public:
	Object() : klass(0) { }
	Object(Object const *P);
	Object(ClassBase const *C, Handle H) : klass(C), handle(H) { }
	virtual ~Object() { }

	virtual void Construct() { }
	virtual void Destroy() { }
	
	void Delete() const;
	bool Exists() const;
	Object *Deref() const;
	Object *Self() const { return Deref(); }

	Factory *GetFactory() const;
	ClassBase const *GetClass() const { return klass; }
	TypeNumber GetTypeNumber() const;
	TypeNumber GetParentTypeNumber() const;
	Handle GetHandle() const { return handle; }

	/// make a new object using this object's factory
	template <class T>
	Object New()
	{
		return GetFactory()->template New<T>();
	}

	/// returns true if this object is of type T
	template <class T>
	bool IsType() const
	{
		return GetTypeNumber() == Traits<T>::Number;
	}
	
	/// return true if this object is of the type T, or its is derived from type T
	template <class T>
	bool IsConvertible() const
	{
		return IsType<T>() || GetParentTypeNumber() == Traits<T>::Number;
	}
};

bool Exists(Object const *);
bool Delete(Object const *);

bool Exists(Object const &);
void Delete(Object const &);

CGL_END

#endif // CGL_OBJECT_H_INCLUDED

//EOF
