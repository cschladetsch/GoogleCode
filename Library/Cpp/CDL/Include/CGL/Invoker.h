// (C) 2009 christian.schladetsch@gmail.com

#ifndef CJS_INVOKER_H
#define CJS_INVOKER_H

CGL_BEGIN

namespace meta
{
	// structure common to all invokers of a given arity. used to define the SinksType
	template <int N>
	struct InvokerBase
	{
		// generates 'class Tn = X' for a given n
		#define CJS_PP_CLASSN_ASSIGN(z, n, X) \
			BOOST_PP_CAT(class T, n) = X

		template <BOOST_PP_ENUM(CJS_ARITY_MAX, CJS_PP_CLASSN_ASSIGN, Null)>
		struct SinksType
		{
			typedef typename Delegate<N>::Given< BOOST_PP_ENUM_PARAMS(CJS_ARITY_MAX, T) > *Ptr;
			typedef std::list<std::pair<DelegateType, Ptr> > Type;
		};
	};

	// This structure adds an operator()(T0, T1,...Tn) to derived structures,
	// and invokes all added delegates when it is called
	template <int>
	struct AddInvoker;

	// use vertical file iteration to deal with the general artiy
	#define BOOST_PP_ITERATION_LIMITS (0, CJS_ARITY_MAX - 1)
	#define BOOST_PP_FILENAME_1       "Detail/AddInvoker.h"
	#include BOOST_PP_ITERATE()
	#undef BOOST_PP_FILENAME_1

} // namespace meta

CGL_END

#endif CJS_INVOKER_H

//EOF
