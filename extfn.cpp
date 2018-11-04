#include <iostream>

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
callchain(const V &v)
{
	return CallchainV<V>(v);
}

template <class F, class V>
struct CallchainFV {
	F &f;
	const V &v;

	CallchainFV(F &f_, const V &v_) : f(f_), v(v_) {}

	CallchainV<typeof(f(v))>
	operator() () const
	{ return callchain(f(v)); }

	template <class T>
	CallchainV<typeof(f(v, T()))>
	operator() (const T &x) const
	{ return callchain(f(v, x)); }

	template <class T1, class T2>
	CallchainV<typeof(f(v, T1(), T2()))>
	operator() (const T1 &x1, const T2 &x2) const
	{ return callchain(f(v, x1, x2)); }

	template <class T1, class T2, class T3>
	CallchainV<typeof(f(v, T1(), T2(), T3()))>
	operator() (const T1 &x1, const T2 &x2, const T3 &x3) const
	{ return callchain(f(v, x1, x2, x3)); }
};

template <class R, class V>
struct CallchainFV {
	typedef R  f_t(V);
	f_t       *f;
	const V &v;

	CallchainFV(F &f_, const V &v_) : f(f_), v(v_) {}

	CallchainV<typeof(f(v))>
	operator() () const
	{ return callchain(f(v)); }

	template <class T>
	CallchainV<typeof(f(v, T()))>
	operator() (const T &x) const
	{ return callchain(f(v, x)); }

	template <class T1, class T2>
	CallchainV<typeof(f(v, T1(), T2()))>
	operator() (const T1 &x1, const T2 &x2) const
	{ return callchain(f(v, x1, x2)); }

	template <class T1, class T2, class T3>
	CallchainV<typeof(f(v, T1(), T2(), T3()))>
	operator() (const T1 &x1, const T2 &x2, const T3 &x3) const
	{ return callchain(f(v, x1, x2, x3)); }
};

template <class R>
CallchainFV
callchainFV(

template <class V>
struct CallchainV {
	const V &v;

	CallchainV(const V &v_) : v(v_) {}

	template <class F>
	CallchainFV<F,V>
	operator() (F &f) const
	{
		return CallchainFV<F,V>(f,v);
	}

	const V &operator() () const
	{
		return v;
	}
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

	int nn =
		callchain(99)
		(foo)
		()
		(bar)
		(88)
		(baz)
		(77)
		()
		;
	std::cout << nn << "\n\n";
	std::cout << baz(bar(foo(99), 88), 77) << "\n";
}

#if 0
	foo(99).bar(88).baz(77) -> baz(bar(foo(99), 88), 77)

	foo(99) ? (bar)(88)
#endif

