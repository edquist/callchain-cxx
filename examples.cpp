#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "cc11-forward.h"

///// "functions" /////

static double dub  (double x)           { return 2*x; }
static double plus (double x, double y) { return x+y; }
static double minus(double x, double y) { return x-y; }
static double times(double x, double y) { return x*y; }
static double divby(double x, double y) { return x/y; }

static double &idub  (double &x)           { return x*=2; }
static double &iplus (double &x, double y) { return x+=y; }
static double &iminus(double &x, double y) { return x-=y; }
static double &itimes(double &x, double y) { return x*=y; }
static double &idivby(double &x, double y) { return x/=y; }

static double &ipow(double &x, double y) { return x = pow(x,y); }
static double &isqrt(double &x)          { return x = sqrt(x); }


static
std::string substr(const std::string &s, size_t pos, size_t len)
{ return s.substr(pos, len); }

static
std::string sconcat(const std::string &s1, const std::string &s2)
{ return s1 + s2; }


static
std::vector<int> v123(int n)
{
    std::vector<int> v(n);
    for (int x = 0; x < n; x++)
	    v[x] = x;
    return v;
}

static
int vsum(const std::vector<int> &v)
{
    int sum = 0;
    for (int x : v)
        sum += x;
    return sum;
}


///// function objects /////

// const operator()
struct Tripler {
    double operator() (double x) const
    { return 3*x; }
};

// non-const operator()
struct DigitAdder {
    int n;
    DigitAdder() : n(1) {}

    // append sequential digits after prev
    int operator() (int prev)
    { return prev * 10 + n++; } 
};


///// tests /////

// nested math functions
double t1_nest()
{
    return sqrt(times(minus(divby(plus(dub(pow(dub(5),3)),14),2),7),10));
}

// chained math functions
double t1_cc1()
{
    double pow(double x, double y);
    double sqrt(double x);

    return callchain(5)
                    (dub)
                    (pow,3)
                    (dub)
                    (plus,14)
                    (divby,2)
                    (minus,7)
                    (times,10)
                    (sqrt)
                    ();
}

// chained math functions, with some lvalue parameters
double t1_cc2()
{
    double pow(double x, double y);
    double sqrt(double x);

    double y = 5;
    double z = 10;

    return callchain(y)
                    (dub)
                    (pow,3)
                    (dub)
                    (plus,14)
                    (divby,2)
                    (minus,7)
                    (times,z)
                    (sqrt)
                    ();
}

// nested in-place math functions
double t2_nest()
{
    double y = 5;
    isqrt(itimes(iminus(idivby(iplus(idub(ipow(idub(y),3)),14),2),7),10));
    return y;
}

// chained in-place math functions
double t2_cc()
{
    double y = 5;
    callchain(y)
             (idub)
             (ipow,3)
             (idub)
             (iplus,14)
             (idivby,2)
             (iminus,7)
             (itimes,10)
             (isqrt)
             ();
    return y;
}


// nested string functions
std::string t3_nest()
{
    std::string xdigits("0123456789abcdef");

    return sconcat(substr(substr(substr(xdigits, 2,12), 5,6), 1,4), "-sdf");
}

// chained string functions
std::string t3_cc1()
{
    std::string xdigits("0123456789abcdef");

    return callchain(xdigits)
                    (substr,2,12)
                    (substr,5,6)
                    (substr,1,4)
                    (sconcat,"-sdf")
                    ();
}

// chained string functions, with const lvalue input
std::string t3_cc2()
{
    const std::string xdigits("0123456789abcdef");

    return callchain(xdigits)
                    (substr,2,12)
                    (substr,5,6)
                    (substr,1,4)
                    (sconcat,"-sdf")
                    ();
}

// chained string functions, with rvalue input
std::string t3_cc3()
{
    return callchain(std::string("0123456789") + "abcdef")
                    (substr,2,12)
                    (substr,5,6)
                    (substr,1,4)
                    (sconcat,"-sdf")
                    ();
}


// nested vector functions
int t4_nest()
{
    return vsum(v123(100));
}

// chained vector functions
int t4_cc1()
{
    return callchain(100)(v123)(vsum)();
}

// chained vector functions, with lvalue input
int t4_cc2()
{
    std::vector<int> v = v123(100);

    return callchain(v)(vsum)();
}


// nested const function objects
double t5_nest()
{
    Tripler trip;

    return trip(dub(trip(5)));
}

// chained const function objects (lvalues)
double t5_cc1()
{
    Tripler trip;

    return callchain(5)(trip)(dub)(trip)();
}

// chained const function objects (rvalues)
double t5_cc2()
{
    return callchain(5)(Tripler())(dub)(Tripler())();
}


// nested non-const function objects
int t6_nest()
{
    DigitAdder dadder;

    return dadder(dadder(dadder(dadder(999))));
}


// chained non-const function objects
int t6_cc()
{
    DigitAdder dadder;

    return callchain(999)(dadder)(dadder)(dadder)(dadder)();
}



int main()
{
    std::cout << "t1_nes : " << t1_nest() << "\n";
    std::cout << "t1_cc1 : " << t1_cc1()  << "\n";
    std::cout << "t1_cc2 : " << t1_cc2()  << "\n";
    std::cout << "\n";
    std::cout << "t2_nest: " << t2_nest() << "\n";
    std::cout << "t2_cc  : " << t2_cc()   << "\n";
    std::cout << "\n";
    std::cout << "t3_nest: " << t3_nest() << "\n";
    std::cout << "t3_cc1 : " << t3_cc1()  << "\n";
    std::cout << "t3_cc2 : " << t3_cc2()  << "\n";
    std::cout << "t3_cc3 : " << t3_cc3()  << "\n";
    std::cout << "\n";
    std::cout << "t4_nest: " << t4_nest() << "\n";
    std::cout << "t4_cc1 : " << t4_cc1()  << "\n";
    std::cout << "t4_cc2 : " << t4_cc2()  << "\n";
    std::cout << "\n";
    std::cout << "t5_nest: " << t5_nest() << "\n";
    std::cout << "t5_cc1 : " << t5_cc1()  << "\n";
    std::cout << "t5_cc2 : " << t5_cc2()  << "\n";
    std::cout << "\n";
    std::cout << "t6_nest: " << t6_nest() << "\n";
    std::cout << "t6_cc  : " << t6_cc()   << "\n";
}

