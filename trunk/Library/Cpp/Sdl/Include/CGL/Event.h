// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_EVENT_H
#define CGL_EVENT_H

#include "CGL/Meta.h"
#include "CGL/Delegate.h"
#include "CGL/Invoker.h"

CGL_BEGIN

namespace event_detail 
{
	using namespace meta;

	#define CJS_EVENT_TYPE_PARAM_LIST \
		BOOST_PP_ENUM_PARAMS(CJS_ARITY_MAX, T)
			
	template <int N>
	struct Event
	{
		template <BOOST_PP_ENUM_PARAMS(CJS_ARITY_MAX, class T)>
		struct Create 
			// add the correct operator() method to the type
			: AddInvoker<N>::template Given<CJS_EVENT_TYPE_PARAM_LIST>	
		{
			typedef typename AddInvoker<N>::template Given<CJS_EVENT_TYPE_PARAM_LIST> Parent;
			typedef typename Parent::Sinks Sinks;
			typedef typename Sinks::value_type ValueType;

			// inject parents' sinks into local scope
			using Parent::sinks;

			/// Create the function delegate type
			typedef typename CreateFunction<N>::template Given<CJS_EVENT_TYPE_PARAM_LIST> FunctionDelegate;	
			typedef typename FunctionDelegate::Function Function;

			/// structure common to both const- and non-const methods
			template <class Class, bool Const>
			struct MethodObjectBase : CreateMethod<N>::Const<Const>::template Given<CJS_EVENT_TYPE_PARAM_LIST>::template Type<Class>
			{
				typedef typename CreateMethod<N>::Const<Const>::template Given<CJS_EVENT_TYPE_PARAM_LIST>::template Type<Class> Parent;
				MethodObjectBase(typename Parent::Servant Q, typename Parent::Method M) : Parent(Q, M) { }
			};

			// a general const method that matches the event signature
			template <class Class>
			struct ConstMethodObject : MethodObjectBase<Class,true>
			{
				typedef typename MethodObjectBase<Class,true> Parent;
				ConstMethodObject(typename Parent::Servant Q, typename Parent::Method M) : Parent(Q, M) { }
			};

			// a general non-const method that matches the event signature
			template <class Class>
			struct MethodObject : MethodObjectBase<Class,false>
			{
				typedef typename MethodObjectBase<Class,false> Parent;
				MethodObject(typename Parent::Servant Q, typename Parent::Method M) : Parent(Q, M) { }
			};

			// remove all delegates on destruction
			~Create()
			{
				Clear();
			}
			
			/// Remove all delegates
			void Clear()
			{
				foreach (typename Sinks::value_type const &delegate, sinks)
					delete delegate.second;
				sinks.clear();	
			}

			/// Add a new function delegate
			void AddFunction(Function fun)
			{
				this->sinks.push_back(ValueType(DelegateType::Function, new FunctionDelegate(fun)));
			}

			/// Add a new Method delegate
			template <class C, bool D>
			void AddMethod(MethodObjectBase<C,D> const &foo)
			{
				this->sinks.push_back(ValueType(DelegateType::Method, new MethodObjectBase<C,D>(foo)));
			}

			/// Remove an existing function delegate
			void RemoveFunction(Function fun)
			{
				typename Sinks::iterator A = this->sinks.begin(), B = this->sinks.end();
				for (; A != B; ++A)
				{
					if (A->first != DelegateType::Function)
						continue;
					FunctionDelegate *F = reinterpret_cast<FunctionDelegate *>(A->second);
					if (F->fun != fun)
						continue;
					delete A->second;
					this->sinks.erase(A);
					return;
				}			
			}

			/// Remove an existing method delegate
			template <class C, bool D>
			void RemoveMethod(MethodObjectBase<C,D> const &bound)
			{
				typename Sinks::iterator A = this->sinks.begin(), B = this->sinks.end();
				for (; A != B; ++A)
				{
					if (A->first != DelegateType::Method)
						continue;
					MethodObjectBase<C,D> *M = reinterpret_cast<MethodObjectBase<C,D> *>(A->second);
					if (M->object != bound.object || M->method != bound.method)
						continue;
					delete A->second;
					this->sinks.erase(A);
					return;
				}
			}

			void Add(Function fun)
			{
				AddFunction(fun);
			}

			template <class C>
			void Add(const C *object, typename ConstMethodObject<C>::Method method)
			{
				AddMethod(ConstMethodObject<C>(object, method));
			}

			template <class C>
			void Add(C *object, typename MethodObject<C>::Method method)
			{
				AddMethod(MethodObject<C>(object, method));
			}

			void Remove(Function fun)
			{
				RemoveFunction(fun);
			}

			template <class C>
			void Remove(const C *object, typename ConstMethodObject<C>::Method method)
			{
				RemoveMethod(object, method);
			}

			template <class C>
			void Remove(C *object, typename MethodObject<C>::Method method)
			{
				RemoveMethod(object, method);
			}
		};
	};
} // end namespace event_detail

/// A multicast event. When the event is fired, all registered delegates
/// will be invoked. A delegate can be either a function pointer, or a const or non-const bound method.
template <BOOST_PP_ENUM(CJS_ARITY_MAX, CJS_PP_CLASSN_ASSIGN, meta::Null)>
struct Event 
	: event_detail::Event<meta::Arity<BOOST_PP_ENUM_PARAMS(CJS_ARITY_MAX, T)>::Value>
		::template Create<BOOST_PP_ENUM_PARAMS(CJS_ARITY_MAX, T)> { };

CGL_END

#endif // CGL_EVENT_H

//EOF
