#include <iostream>
#include "propertypp.h"

class Example
{
    public:
        Example();
        void printProperties();
        PROPERTYPP_DECLARE(int, i, i_)
        PROPERTYPP_DECLARE(float, f, f_)
        PROPERTYPP_DECLARE(int*, p, p_)
};

Example::Example()
{
    PROPERTYPP_INIT(i);
    PROPERTYPP_INIT(f);
    PROPERTYPP_INIT(p);
}

void Example::printProperties()
{
    std::cout << "i: " << i
              << "; f: " << f
              << "; p: " << p << " (" << (p ? *p : 0) << ")"
              << std::endl;
}

PROPERTYPP_SYNTHESIZE(Example, int, i, i_)

PROPERTYPP_SYNTHESIZE_GETTER(Example, float, f, f_)
void Example::setf(float value)
{
    if (value > 0) f_ = value;
}

PROPERTYPP_SYNTHESIZE(Example, int*, p, p_)

int main()
{
    Example e;
    e.printProperties(); // should print all 0

    e.i = 14;
    e.f = 2.3;
    e.p = new int;
    *e.p = 5;
    e.printProperties(); // should print assigned values

    e.f = -1.0;
    e.printProperties(); // e.f should be unchanged (see setter above)

    e.i = e.f;           // implicit type conversion
    e.printProperties(); // e.i should equal the integer part of e.f

    e.f = 5.5;
    e.printProperties(); // only e.f should be changed

    e.i = e.f = 9.3;     // chained assignment
    e.printProperties(); // e.i should be 9, e.f should be 9.3

    e.i = *e.p;
    e.printProperties(); // e.i should equal the value pointed to by e.p

    delete e.p;
    e.p = 0;
    e.printProperties(); // e.p should be null

    return 0;
}
