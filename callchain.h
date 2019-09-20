#pragma once

#include <utility>

template <class V> struct Callchain;
template <class F, class V> struct CallchainFV;

template <class V>
  inline Callchain<V> callchain(V &&x)
{ return Callchain<V>(std::forward<V>(x)); }

template <class V>
struct Callchain {
    V value;

    Callchain(V &&x) : value(std::forward<V>(x)) {}

    // accept a function in brackets
    template <class F>
    CallchainFV<F,V> operator[] (F &&f)
    { return CallchainFV<F,V>(std::forward<F>(f), std::forward<V>(value)); }

    // call a function with value and any additional args
    template <class F, class ...Args>
    auto operator() (F &&f, Args &&...args)
      -> decltype(callchain(f(std::forward<V>(value),
                              std::forward<Args>(args)...)))
    {
        return callchain(f(std::forward<V>(value),
                           std::forward<Args>(args)...));
    }

    V &&operator() ()
    { return std::forward<V>(value); }
};

// callchain(x)(foo)(bar,y,z)(baz)() --> baz(bar(foo(x),y,z))



template <class F, class V>
struct CallchainFV {
    F &&f;
    V &&value;

    CallchainFV(F &&f, V &&v)
        : f(std::forward<F>(f)), value(std::forward<V>(v)) {}

    // call the function with value and any additional args
    template <class ...Args>
    auto operator() (Args &&...args)
      -> decltype(callchain(f(std::forward<V>(value),
                              std::forward<Args>(args)...)))
    {
        return callchain(f(std::forward<V>(value),
                           std::forward<Args>(args)...));
    }
};

// callchain(x)[foo]()[bar](y,z)[baz]()() --> baz(bar(foo(x),y,z))

