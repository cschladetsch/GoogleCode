// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_DEREFERENCE_H_INCLUDED
#	define CGL_DEREFERENCE_H_INCLUDED

CGL_BEGIN

namespace detail
{
	/// deref for the non-abstract case
	template <class T, bool Abstract>
	struct DerefBase
	{
		static typename Traits<T>::PointerType From(Object *base)
		{
			Storage<T> *storage = static_cast<Storage<T> *>(base);
			return &storage->GetStorage();
		}
	};

	/// deref for the abstract case
	template <class T>
	struct DerefBase<T, true>
	{
		static typename Traits<T>::PointerType From(Object *base)
		{
			Storage<BaseBase> *base_storage = reinterpret_cast<Storage<BaseBase> *>(base);
			BaseBase *base_ptr = &base_storage->GetStorage();
			// WHAT THE HELL IS THIS !!!
			// for some reason, the ptr for Bullets is 4 bytes along... i know there is a good
			// reason for this, but other objects (Towers, Runners etc) are fine.. just Bullets are screwy (so far)
			if (base->GetTypeNumber() == 107)
			{
				base_ptr++;
			}
			return static_cast<typename Traits<T>::PointerType>(base_ptr);
		}
	};
}

/// @brief dereference an object to the given type
///
/// the implementation is provided by derivation from detail::DerefBase<>
/// which is specialised for the case of abstract and non-abstract types
template <class T>
struct Dereference : detail::DerefBase<T, Traits<T>::Abstract> { };

CGL_END

#endif // CGL_DEREFERENCE_H_INCLUDED

//EOF
