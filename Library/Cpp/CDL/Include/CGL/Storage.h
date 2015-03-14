// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_STORAGE_BASE_H_INCLUDED
#	define CGL_STORAGE_BASE_H_INCLUDED

/// storage for an object of a given type T
template <class T>
struct Storage : Object
{
	typedef Traits<T> MyTraits;
	typedef typename MyTraits::StorageType StorageType;

private:
	StorageType storage;

public:
	Storage(ClassBase const *C, Handle H) : Object(C, H) { }

	StorageType &GetStorage() { return storage; }
	StorageType const &GetStorage() const { return storage; }
};

#endif // CGL_STORAGE_BASE_H_INCLUDED

CGL_BEGIN
