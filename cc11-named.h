#pragma once

template <class T> struct Callchain;

template <class T>
  inline Callchain<T> callchain(T x)
{ return Callchain<T>(x); }

template <class T>
struct Callchain {
    T value;

    Callchain(T x) : value(x) {}

    template <class F, class ...Args>
    auto apply(F &f, Args ...args)
      -> decltype(callchain(f(value, args...)))  // for pre C++14
    { return callchain(f(value, args...)); }

    T result()
    { return value; }
};

// callchain(x).apply(f).apply(g,y,z).apply(h).result() --> h(g(f(x),y,z))

