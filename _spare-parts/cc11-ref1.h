#pragma once

#include <utility>

template <class T> struct Callchain;
template <class T> struct const_Callchain;

template <class T>
inline const_Callchain<T> callchain(const T &x)
{ return const_Callchain<T>(x); }

template <class T>
inline Callchain<T> callchain(T &x)
{ return Callchain<T>(x); }

template <class T>
struct const_Callchain {
    const T &value;

    const_Callchain(const T &x) : value(x) {}

    template <class F, class ...Args>
    auto operator() (F &f, Args &&...args)
      -> decltype(callchain(f(value, std::forward<Args>(args)...)))
    {
        return callchain(f(value, std::forward<Args>(args)...));
    }

    const T &operator() ()
    { return value; }
};

template <class T>
struct Callchain {
    T &value;

    Callchain(T &x) : value(x) {}

    template <class F, class ...Args>
    auto operator() (F &f, Args &&...args)
      -> decltype(callchain(f(value, std::forward<Args>(args)...)))
    {
        return callchain(f(value, std::forward<Args>(args)...));
    }

    T &operator() ()
    { return value; }
};

// callchain(x)(foo)(bar,y,z)(baz)() --> baz(bar(foo(x),y,z))

