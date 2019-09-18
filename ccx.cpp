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
        std::cout << *this << "[in destructor]\n";
        value = -2;
        state = INVALID;
        std::cout << *this << "[destroyed]\n";
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
    Item r = times(iminus(divby(dub(plus(dub(Item(5)), 14)), 2), 7), 10);
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
    std::cout << "\n";
    std::cout << "t2_cc: " << t2_cc() << "\n";
    std::cout << "\n";
    std::cout << "t2_x:  " << t2_x() << "\n";
}

