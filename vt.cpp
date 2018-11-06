#include <iostream>
#include <string>
#include <math.h>

template <class T> struct Callchain;

template <class T>
Callchain<T> callchain(const T &x)
{ return Callchain<T>(x); }

template <class T>
struct Callchain {
    T value;

    Callchain(const T &x) : value(x) {}

    template <class F, class... Args>
    auto operator() (F &f, Args... args)
      -> decltype(callchain(f(value, args...)))
    { return callchain(f(value, args...)); }
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
    std::cout << callchain(5)(dub)
                             (pow,3)
                             (dub)
                             (plus,14)
                             (divby,2)
                             (minus,7)
                             (times,10)
                             (sqrt)
                             .value << "\n";

    std::cout << sqrt(times(minus(divby(plus(dub(pow(dub(5),3)),14),2),7),10))
              << "\n";

    std::string xdigits("0123456789abcdef");

    std::cout << substr(substr(substr(xdigits, 2,12), 5,6), 1,4);

    std::cout << "\n";

    std::cout << callchain(xdigits)(substr,2,12)
                                   (substr,5,6)
                                   (substr,1,4)
                                   .value;
    std::cout << "\n";
}

