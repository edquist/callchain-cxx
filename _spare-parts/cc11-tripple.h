#pragma once

#include <utility>

template <class T> struct Callchain;

template <class T>
Callchain<T> callchain(const T &x)
{ return Callchain<T>(x); }

template <class T>
Callchain<T> callchain(T &x)
{ return Callchain<T>(x); }

template <class T>
Callchain<T> callchain(T &&x)
{ return Callchain<T>(std::move(x)); }


template <class T>
struct Callchain {
    T value;

    Callchain(const T &x) : value(x) {}
    Callchain(T &x)       : value(x) {}
    Callchain(T &&x)      : value(std::move(x)) {}

    template <class F, class... Args>
    auto operator() (F &f, Args &&...args)
      -> decltype(callchain(f(std::move(value), std::forward<Args>(args)...)))
         // for pre c++14
    { return callchain(f(std::move(value), std::forward<Args>(args)...)); }

    T &&operator() ()
    { return std::move(value); }
};

