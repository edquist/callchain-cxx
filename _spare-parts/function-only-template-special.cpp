#include <iostream>

template <class F>
struct FnRet;
//{
//    void boo() { std::cout << "base boo\n"; }
//};

template <>
template <class R>
struct FnRet<R()> {
    void boo() { std::cout << "special boo\n"; }
};


int main()
{
//  FnRet<int> x;
//  x.boo();

    FnRet<void()> y;
    y.boo();
}
