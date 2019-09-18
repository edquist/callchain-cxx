
// c++98 way to get std::functional<F>::result_type

template <class F> struct FnRet;

template <> template <class R, class A1>
struct FnRet<R(A1)> { typedef R result_type; };

template <> template <class R, class A1, class A2>
struct FnRet<R(A1,A2)> { typedef R result_type; };

template <> template <class R, class A1, class A2, class A3>
struct FnRet<R(A1,A2,A3)> { typedef R result_type; };

