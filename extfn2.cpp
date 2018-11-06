#include <iostream>
#include <math.h>

template <class V>
struct Callchain {
	V v;

	Callchain(V v_) : v(v_) {}

	template <class R, class A1>
	Callchain<R>
	operator() (R (*f)(A1))
	{ return Callchain<R>(f(v)); }

	template <class R, class A1, class A2, class V2>
	Callchain<R>
	operator() (R (*f)(A1,A2), V2 v2)
	{ return Callchain<R>(f(v,v2)); }

	template <class R, class A1, class A2, class A3, class V2, class V3>
	Callchain<R>
	operator() (R (*f)(A1,A2,A3), V2 v2, V3 v3)
	{ return Callchain<R>(f(v,v2,v3)); }

	V operator() () const
	{ return v; }
};


// start callchain with a value

template <class V>
  inline Callchain<V> callchain(V val)
{ return Callchain<V>(val); }


// functions for examples

int foo(int x) { return x * 10000; }

int bar(int x, int y) { return x + 100 * y; }

int baz(int x, int y) { return x + y; }

double dub  (double x)           { return 2*x; }
double plus (double x, double y) { return x+y; }
double minus(double x, double y) { return x-y; }
double times(double x, double y) { return x*y; }
double divby(double x, double y) { return x/y; }

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
    // (5).dub().pow(3).dub().plus(14).divby(2).minus(7).times(10).sqrt()

    std::cout << sqrt(times(minus(divby(plus(dub(pow(dub(5),3)),14),2),7),10));

    std::cout << "\n";

    std::cout << callchain(5)(dub)
                             (pow,3)
                             (dub)
                             (plus,14)
                             (divby,2)
                             (minus,7)
                             (times,10)
                             (sqrt)
                             ();

    std::cout << "\n\n---\n\n";
}

void ex4()
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
	ex4();
}

