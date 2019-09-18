#pragma once

#include <utility>

template <class T> struct Callchain;

template <class T>
  inline Callchain<T> callchain(T &&x)
{ return Callchain<T>(std::forward<T>(x)); }

template <class T>
struct Callchain {
    T value;

    Callchain(T &&x) : value(std::forward<T>(x)) {}

    template <class F, class ...Args>
    auto operator() (F &f, Args &&...args)
      -> decltype(callchain(f(std::forward<T>(value),
                              std::forward<Args>(args)...)))
    {
        return callchain(f(std::forward<T>(value),
                           std::forward<Args>(args)...));
    }

    T &&operator() ()
    { return std::forward<T>(value); }
};

// callchain(x)(foo)(bar,y,z)(baz)() --> baz(bar(foo(x),y,z))

