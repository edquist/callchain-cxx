#pragma once

#include <utility>

template <class V>          struct CallchainV;
template <class F, class V> struct CallchainFV;

template <class V>
  inline CallchainV<V> callchain2(V &&v)
{ return CallchainV<V>(std::forward<V>(v)); }

template <class V>
struct CallchainV {
    V value;

    CallchainV(V &&v) : value(std::forward<V>(v)) {}

    template <class F>
    CallchainFV<F,V> operator[] (F &&f)
    { return CallchainFV<F,V>(std::forward<F>(f), std::forward<V>(value)); }

    V &&operator() ()
    { return std::forward<V>(value); }
};

template <class F, class V>
struct CallchainFV {
    F &&f;
    V &&value;

    CallchainFV(F &&f, V &&v)
        : f(std::forward<F>(f)), value(std::forward<V>(v)) {}

    template <class ...Args>
    auto operator() (Args &&...args)
      -> decltype(callchain2(f(std::forward<V>(value),
                               std::forward<Args>(args)...)))
    {
        return callchain2(f(std::forward<V>(value),
                            std::forward<Args>(args)...));
    }
};

// callchain2(x)[foo]()[bar](y,z)[baz]()() --> baz(bar(foo(x),y,z))

