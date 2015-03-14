// (C) 2009 christian.schladetsch@gmail.com

#ifndef FACTORY_H_INCLUDED
#	define FACTORY_H_INCLUDED

CGL_BEGIN

/// stores a set of classes and instances
struct Factory
{
	typedef std::map<TypeNumber, ClassBase *> Classes;
	typedef std::map<Handle, Object *> Instances;
	typedef std::set<Handle> Deathrow;

	Classes classes;
	Instances instances;
	Deathrow deathrow;
	Handle next_handle;

public:
	Factory();
	~Factory();

	/// register a class with the factory
	template <class T>
	void AddClass()
	{
		classes[Traits<T>::Number] = new Class<T>(this);
	}

	/// make a new object given its type
	template <class T>
	Pointer<T> New()
	{
		Classes::const_iterator klass = classes.find(Traits<T>::Number);
		assert(klass != classes.end());
		if (klass == classes.end())
			return 0;
		return NewFromClass(klass->second);
	}

	/// make a new object given its type number
	void New(TypeNumber N);

	/// make a new object from its class-base
	Object *NewFromClass(ClassBase *);

	/// retreive an object given its handle
	Object *GetObject(Handle handle) const;

	/// delete an existing object.
	/// resources will be deleted on the next PurSge (see Factory::Purge)
	void Delete(Object *);
	void Delete(Handle handle);

	/// return true if the given object `exists`.
	/// when an object is Deleted as above, it still exists
	/// in system memory until the Purge method is called
	/// on the factory that made the object.
	/// Exists() returns false for objects that:
	///		* have never been created
	///		* have been created and have been released
	///		* are currently created, but have been deleted, waiting for next Purge
	bool Exists(Object const &) const;
	bool Exists(Handle) const;

	/// release resources for all objects pending deletion
	void Purge();
};

CGL_END

#endif // FACTORY_H_INCLUDED

//EOF
