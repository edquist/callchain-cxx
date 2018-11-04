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

template <class R, class V>
struct CallchainFV1 {
	typedef R  fn_t(V);
	fn_t      *fn;
	const V   &val;

	CallchainFV1(fn_t *fn_, const V &val_) : fn(fn_), val(val_)
	{
		std::cout << "[CallchainFV1(" << val << ")]\n";
	}

	CallchainV<R>
	operator() () const
	{ return CallchainV<R>(fn(val)); }
};

template <class R, class V, class A2>
struct CallchainFV2 {
	typedef R  fn_t(V, A2);
	fn_t      *fn;
	const V   &val;

	CallchainFV2(fn_t *fn_, const V &val_) : fn(fn_), val(val_)
	{
		std::cout << "[CallchainFV2(" << val << ")]\n";
	}

	CallchainV<R>
	operator() (const A2 &a2) const
	{ return CallchainV<R>(fn(val, a2)); }
};

template <class R, class V, class A2, class A3>
struct CallchainFV3 {
	typedef R  fn_t(V, A2, A3);
	fn_t      *fn;
	const V   &val;

	CallchainFV3(fn_t *fn_, const V &val_) : fn(fn_), val(val_)
	{
		std::cout << "[CallchainFV3(" << val << ")]\n";
	}

	CallchainV<R>
	operator() (const A2 &a2, const A3 &a3) const
	{ return CallchainV<R>(fn(val, a2, a3)); }
};

template <class V>
struct CallchainV {
	const V &v;

	CallchainV(const V &v_) : v(v_)
	{
		std::cout << "[CallchainV(" << v << ")]\n";
	}

	template <class R>
	CallchainFV1<R,V>
	operator() (R (*f)(V))
	{ return CallchainFV1<R,V>(f,v); }

	template <class R, class A2>
	CallchainFV2<R,V,A2>
	operator() (R (*f)(V,A2))
	{
		std::cerr << "[CallchainV (FV2) v=" << v << "]\n";
		return CallchainFV2<R,V,A2>(f,v); }

	template <class R, class A2, class A3>
	CallchainFV3<R,V,A2,A3>
	operator() (R (*f)(V,A2,A3))
	{ return CallchainFV3<R,V,A2,A3>(f,v); }

	const V &operator() () const
	{
		return v;
	}
};

int foo(int x) {
	std::cerr << "[hi from foo(" << x << ")]\n";
	return x * 10000; }

int bar(int x, int y) {
	std::cerr << "[hi from bar(" << x << "," << y << ")]\n";
	return x + 100 * y; }

int baz(int x, int y) {
	std::cerr << "[hi from baz(" << x << "," << y << ")]\n";
	return x + y; }

int main()
{
	BOO	("hi")
		("There")
		("Mr")
		("Man");

	int nn = callchain(99)
		(foo)()
		(bar)(88)
		(baz)(77)
		();
	std::cout << nn << "\n\n";
	std::cout << baz(bar(foo(99), 88), 77) << "\n";
}

#if 0
	foo(99).bar(88).baz(77) -> baz(bar(foo(99), 88), 77)

	foo(99) ? (bar)(88)
#endif

