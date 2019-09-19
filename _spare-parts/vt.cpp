#include <iostream>
#include <string>
#include <utility>
#include <math.h>

template <class T> struct Callchain;

#if 0

template <class T>
Callchain<T> callchain(T x)
{ return Callchain<T>(x); }

#else

template <class T>
Callchain<T> callchain(const T &x)
{ return Callchain<T>(x); }

template <class T>
Callchain<T> callchain(T &x)
{ return Callchain<T>(x); }

template <class T>
Callchain<T> callchain(T &&x)
{ return Callchain<T>(std::move(x)); }

#endif

template <class T>
struct Callchain {
    T value;

#if 0
    Callchain(T x)        : value(x) {}
#else
    Callchain(const T &x) : value(x) {}
    Callchain(T &x)       : value(x) {}
    Callchain(T &&x)      : value(std::move(x)) {}
#endif

#if 0
    template <class F, class... Args>
    auto operator() (F &f, Args... args)
      -> decltype(callchain(f(std::move(value), args...)))  // for pre c++14
    { return callchain(f(std::move(value), args...)); }
#else
    template <class F, class... Args>
    auto operator() (F &f, Args &&...args)
      -> decltype(callchain(f(std::move(value), std::forward<Args>(args)...)))
         // for pre c++14
    { return callchain(f(std::move(value), std::forward<Args>(args)...)); }
#endif

    T &&operator() ()
    { return std::move(value); }
};

double dub  (double x)           { return 2*x; }
double plus (double x, double y) { return x+y; }
double minus(double x, double y) { return x-y; }
double times(double x, double y) { return x*y; }
double divby(double x, double y) { return x/y; }

std::string substr(std::string s, size_t pos, size_t len)
{
    return s.substr(pos, len);
}

int main()
{
    double pow(double x, double y);
    double sqrt(double x);

    std::cout << callchain(5)
                          (dub)
                          (pow,3)
                          (dub)
                          (plus,14)
                          (divby,2)
                          (minus,7)
                          (times,10)
                          (sqrt)
                          () << "\n";

    double z = 10;

    std::cout << callchain(5)
                          (dub)
                          (pow,3)
                          (dub)
                          (plus,14)
                          (divby,2)
                          (minus,7)
                          (times,z)
                          (sqrt)
                          () << "\n";

    std::cout << sqrt(times(minus(divby(plus(dub(pow(dub(5),3)),14),2),7),10))
              << "\n";

    std::string xdigits("0123456789abcdef");

    std::cout << substr(substr(substr(xdigits, 2,12), 5,6), 1,4);

    std::cout << "\n";

    std::cout << callchain(xdigits)
                          (substr,2,12)
                          (substr,5,6)
                          (substr,1,4)
                          ();
    std::cout << "\n";
}

