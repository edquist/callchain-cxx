#pragma once

#include <utility>

template <class T> struct Callchain;

template <class T>
inline //Callchain<T>
auto callchain(T &&x)
  -> Callchain<decltype(std::forward<T>(x))>
{
    return Callchain<decltype(std::forward<T>(x))>
           (std::forward<T>(x));
}

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

    //T &&
    decltype(std::forward<T>(value)) operator() ()
    { return std::forward<T>(value); }
};

// callchain(x)(foo)(bar,y,z)(baz)() --> baz(bar(foo(x),y,z))

