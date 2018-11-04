#include <iostream>
#include <math.h>

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

	template <class R, class A1>
	CallchainFV1<R,A1>
	operator() (R (*f)(A1))
	{ return CallchainFV1<R,A1>(f,v); }

	template <class R, class A1, class A2>
	CallchainFV2<R,A1,A2>
	operator() (R (*f)(A1,A2))
	{ return CallchainFV2<R,A1,A2>(f,v); }

	template <class R, class A1, class A2, class A3>
	CallchainFV3<R,A1,A2,A3>
	operator() (R (*f)(A1,A2,A3))
	{ return CallchainFV3<R,A1,A2,A3>(f,v); }

	V operator() () const
	{ return v; }
};


template <class R, class V>
struct CallchainF1 {
	typedef R  fn_t(V);
	fn_t      *fn;
	V          val;

	CallchainF1(fn_t *fn_) : fn(fn_) {}

	CallchainV<R>
	operator() (V val) const
	{ return CallchainV<R>(fn(val)); }
};

template <class R, class V, class A2>
struct CallchainF2 {
	typedef R  fn_t(V, A2);
	fn_t      *fn;
	V          val;

	CallchainF2(fn_t *fn_) : fn(fn_) {}

	CallchainV<R>
	operator() (V val, A2 a2) const
	{ return CallchainV<R>(fn(val, a2)); }
};

template <class R, class V, class A2, class A3>
struct CallchainF3 {
	typedef R  fn_t(V, A2, A3);
	fn_t      *fn;
	V          val;

	CallchainF3(fn_t *fn_) : fn(fn_) {}

	CallchainV<R>
	operator() (V val, A2 a2, A3 a3) const
	{ return CallchainV<R>(fn(val, a2, a3)); }
};

template <class R, class V>
CallchainF1<R,V>
callchainF(R (*f)(V))
{ return CallchainF1<R,V>(f); }

template <class R, class V, class A2>
CallchainF2<R,V,A2>
callchainF(R (*f)(V,A2))
{ return CallchainF2<R,V,A2>(f); }

template <class R, class V, class A2, class A3>
CallchainF3<R,V,A2,A3>
callchainF(R (*f)(V,A2,A3))
{ return CallchainF3<R,V,A2,A3>(f); }


// functions for examples

int foo(int x) { return x * 10000; }

int bar(int x, int y) { return x + 100 * y; }

int baz(int x, int y) { return x + y; }


std::string substr(std::string s, size_t pos, size_t len)
{
	return s.substr(pos, len);
}


void ex1()
{
	// foo(99).bar(88).baz(77)

	std::cout << "\n";

	std::cout << baz(bar(foo(99), 88), 77);

	std::cout << "\n";

	std::cout << callchainF(foo)(99)
	                       (bar)(88)
	                       (baz)(77)
	                       ();
	std::cout << "\n";

	std::cout << callchain(99)(foo)()
	                          (bar)(88)
	                          (baz)(77)
	                          ();

	std::cout << "\n\n---\n\n";
}

void ex2()
{
	// sqrt(12345.67).sqrt().sqrt()

	std::cout << sqrt(sqrt(sqrt(12345.67)));

	std::cout << "\n";

	std::cout << callchainF(sqrt)(12345.67)
	                       (sqrt)()
	                       (sqrt)()
	                       ();

	std::cout << "\n";

	std::cout << callchain(12345.67)(sqrt)()
	                                (sqrt)()
	                                (sqrt)()
	                                ();
	std::cout << "\n\n---\n\n";
}

void ex3()
{
	// substr(xdigits,2,12).substr(5,6).substr(1,4)

	std::string xdigits("0123456789abcdef");

	std::cout << substr(substr(substr(xdigits, 2,12), 5,6), 1,4);

	std::cout << "\n";

	std::cout << callchainF(substr)(xdigits,2,12)
	                       (substr)(5,6)
	                       (substr)(1,4)
	                       ();
	std::cout << "\n";

	std::cout << callchain(xdigits)(substr)(2,12)
	                               (substr)(5,6)
	                               (substr)(1,4)
	                               ();
	std::cout << "\n\n---\n\n";
}

int main()
{
	ex1();
	ex2();
	ex3();
}

