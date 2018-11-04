#include <iostream>
#include <math.h>

template <class V>
struct CallchainV {
	V v;

	CallchainV(V v_) : v(v_) {}

	template <class R, class A1>
	CallchainV<R>
	operator() (R (*f)(A1))
	{ return CallchainV<R>(f(v)); }

	template <class R, class A1, class A2, class V2>
	CallchainV<R>
	operator() (R (*f)(A1,A2), V2 v2)
	{ return CallchainV<R>(f(v,v2)); }

	template <class R, class A1, class A2, class A3, class V2, class V3>
	CallchainV<R>
	operator() (R (*f)(A1,A2,A3), V2 v2, V3 v3)
	{ return CallchainV<R>(f(v,v2,v3)); }

	V operator() () const
	{ return v; }
};


// start callchain with a value

template <class V>
  inline CallchainV<V> callchain(V val)
{ return CallchainV<V>(val); }


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

	std::cout << callchain(99)(foo)
	                          (bar,88)
	                          (baz,77)
	                          ();

	std::cout << "\n\n---\n\n";
}

void ex2()
{
	// sqrt(12345.67).sqrt().sqrt()

	std::cout << sqrt(sqrt(sqrt(12345.67)));

	std::cout << "\n";

	std::cout << callchain(12345.67)(sqrt)
	                                (sqrt)
	                                (sqrt)
	                                ();
	std::cout << "\n\n---\n\n";
}

void ex3()
{
	// substr(xdigits,2,12).substr(5,6).substr(1,4)

	std::string xdigits("0123456789abcdef");

	std::cout << substr(substr(substr(xdigits, 2,12), 5,6), 1,4);

	std::cout << "\n";

	std::cout << callchain(xdigits)(substr,2,12)
	                               (substr,5,6)
	                               (substr,1,4)
	                               ();
	std::cout << "\n\n---\n\n";
}

int main()
{
	ex1();
	ex2();
	ex3();
}

