#include <iostream>
#include <string>
#include <math.h>

#include "cc11-forward.h"
//#include "cc11-simple.h"

struct Item {
    typedef double what;

    what value;
    enum State { INVALID = 0, VALID, GUTTED };
    State state;

    what get() const
    {
        if (state != VALID)
            std::cout << *this << "[BAD GET REQUEST]\n";
        return value;
    }

    friend std::ostream &operator<< (std::ostream &os, const Item &x)
    {
        return os << "[Item@" << &x
                  << ": value=" << x.value
                  << ", state=" << x.state
                  << "]";
    }

    Item(what x) : value(x), state(VALID)
    {
        std::cout << *this << "[initialized by value]\n";
    }

    Item(const Item &x) : value(x.value), state(VALID)
    {
        if (x.state != VALID)
            std::cout << "[BAD COPY REQUEST]\n";
        std::cout << *this << "[initialized by copy from " << x << "]\n";
    }

    Item(Item &&x) : value(x.value), state(VALID)
    {
        if (x.state != VALID)
            std::cout << "[BAD MOVE REQUEST]\n";
        x.value = -1;
        x.state = GUTTED;
        std::cout << *this << "[initialized by move from " << x << "]\n";
    }

    Item &operator= (const Item &x)
    {
        if (x.state != VALID)
            std::cout << "[BAD COPY-ASSIGN REQUEST]\n";
        value = x.value;
        state = VALID;
        std::cout << *this << "[copy-assigned from " << x << "]\n";
        return *this;
    }

    Item &operator= (Item &&x)
    {
        if (x.state != VALID)
            std::cout << "[BAD MOVE-ASSIGN REQUEST]\n";
        value = x.value;
        state = VALID;
        x.value = -1;
        x.state = GUTTED;
        std::cout << *this << "[move-assigned from " << x << "]\n";
        return *this;
    }

    ~Item()
    {
        std::cout << *this << "[destroying]\n";
        value = -2;
        state = INVALID;
    }

};

Item dub   (Item x)         { return Item(2*x.get()); }
Item iplus (Item x, Item y) { return Item(x.get()+y.get()); }
Item iminus(Item x, Item y) { return Item(x.get()-y.get()); }
Item itimes(Item x, Item y) { return Item(x.get()*y.get()); }
Item idivby(Item x, Item y) { return Item(x.get()/y.get()); }

Item plus (Item x, Item::what y) { return Item(x.get()+y); }
Item minus(Item x, Item::what y) { return Item(x.get()-y); }
Item times(Item x, Item::what y) { return Item(x.get()*y); }
Item divby(Item x, Item::what y) { return Item(x.get()/y); }

Item &xdub  (Item &x)               { x.value*=2; return x; }
Item &xplus (Item &x, Item::what y) { x.value+=y; return x; }
Item &xminus(Item &x, Item::what y) { x.value-=y; return x; }
Item &xtimes(Item &x, Item::what y) { x.value*=y; return x; }
Item &xdivby(Item &x, Item::what y) { x.value/=y; return x; }
Item &ximinus(Item &x, Item &y) { x.value-=y.value; y.value *= -1; return x; }

Item t1_cc()
{
    return callchain(Item(5))
                    (dub)
                    (plus,14)
                    (dub)
                    (divby,2)
                    (minus,7)
                    (times,10)
                    ();
}

Item t1_x()
{
    return times(minus(divby(dub(plus(dub(Item(5)), 14)), 2), 7), 10);
}

Item t2_cc()
{
    Item i5(5), i7(7);

    Item r = callchain(i5)
                    (dub)
                    (plus,14)
                    (dub)
                    (divby,2)
                    (iminus,i7)
                    (times,10)
                    ();
    std::cout << "[i5=" << i5 << "]\n";
    std::cout << "[i7=" << i7 << "]\n";
    return r;
}

Item t2_x()
{
    Item i5(5), i7(7);
    Item r = times(iminus(divby(dub(plus(dub(i5), 14)), 2), i7), 10);
    std::cout << "[i5=" << i5 << "]\n";
    std::cout << "[i7=" << i7 << "]\n";
    return r;
}

Item t3_cc()
{
    Item i5(5), i7(7);

    Item r = callchain(i5)
                      (xdub)
                      (xplus,14)
                      (xdub)
                      (xdivby,2)
                      (ximinus,i7)
                      (xtimes,10)
                      ();
    std::cout << "[i5=" << i5 << "]\n";
    std::cout << "[i7=" << i7 << "]\n";
    return r;
}

Item t3_x()
{
    Item i5(5), i7(7);
    Item r = times(ximinus(xdivby(xdub(xplus(xdub(i5), 14)), 2), i7), 10);
    std::cout << "[i5=" << i5 << "]\n";
    std::cout << "[i7=" << i7 << "]\n";
    return r;
}

int main()
{

    std::cout << "t1_cc: " << t1_cc() << "\n";
    std::cout << "\n";
    std::cout << "t1_x:  " << t1_x() << "\n";
    std::cout << "\n";
    std::cout << "---\n";
    std::cout << "\n";
    std::cout << "t2_cc: " << t2_cc() << "\n";
    std::cout << "\n";
    std::cout << "t2_x:  " << t2_x() << "\n";
    std::cout << "\n";
    std::cout << "---\n";
    std::cout << "\n";
    std::cout << "t3_cc: " << t3_cc() << "\n";
    std::cout << "\n";
    std::cout << "t3_x:  " << t3_x() << "\n";
}

