#include <iostream>
#include <math.h>

struct Boo {
	template <class T>
	const Boo &operator() (T &thing) const
	{
		std::cout << thing << "\n";
		return *this;
	}
} BOO;

template <class V>
struct CallchainV;

template <class V>
CallchainV<V>
callchain(V v)
{
	return CallchainV<V>(v);
}

template <class R, class V>
struct CallchainFV1 {
	typedef R  fn_t(V);
	fn_t      *fn;
	V          val;

	CallchainFV1(fn_t *fn_, V val_) : fn(fn_), val(val_) {}

	CallchainV<R>
	operator() () const
	{ return CallchainV<R>(fn(val)); }
};

template <class R, class V, class A2>
struct CallchainFV2 {
	typedef R  fn_t(V, A2);
	fn_t      *fn;
	V          val;

	CallchainFV2(fn_t *fn_, V val_) : fn(fn_), val(val_) {}

	CallchainV<R>
	operator() (A2 a2) const
	{ return CallchainV<R>(fn(val, a2)); }
};

template <class R, class V, class A2, class A3>
struct CallchainFV3 {
	typedef R  fn_t(V, A2, A3);
	fn_t      *fn;
	V          val;

	CallchainFV3(fn_t *fn_, V val_) : fn(fn_), val(val_) {}

	CallchainV<R>
	operator() (A2 a2, A3 a3) const
	{ return CallchainV<R>(fn(val, a2, a3)); }
};

template <class V>
struct CallchainV {
	V v;

	CallchainV(V v_) : v(v_) {}

	template <class R>
	CallchainFV1<R,V>
	operator() (R (*f)(V))
	{ return CallchainFV1<R,V>(f,v); }

	template <class R, class A2>
	CallchainFV2<R,V,A2>
	operator() (R (*f)(V,A2))
	{ return CallchainFV2<R,V,A2>(f,v); }

	template <class R, class A2, class A3>
	CallchainFV3<R,V,A2,A3>
	operator() (R (*f)(V,A2,A3))
	{ return CallchainFV3<R,V,A2,A3>(f,v); }

	V operator() () const
	{ return v; }
};

int foo(int x) { return x * 10000; }

int bar(int x, int y) { return x + 100 * y; }

int baz(int x, int y) { return x + y; }

int main()
{
	BOO	("hi")
		("There")
		("Mr")
		("Man");

	std::cout << "\n\n";

	int nn = callchain(99)(foo)()
		              (bar)(88)
		              (baz)(77)
		              ();

	std::cout << nn << "\n\n";

	std::cout << baz(bar(foo(99), 88), 77) << "\n";

	std::cout << "\n\n";

	std::cout << sqrt(sqrt(sqrt(12345.67)));

	std::cout << "\n\n";

	std::cout << callchain(12345.67)(sqrt)()
	                                (sqrt)()
	                                (sqrt)()
	                                ();
	std::cout << "\n\n";

}

//	foo(99).bar(88).baz(77) -> baz(bar(foo(99), 88), 77)

