#include <cassert>
#include <iostream>
#include <string>
#include "propertypp.h"

class Test
{
    public:
        Test();
        Test(const Test& rhs);
        PROPERTYPP_DECLARE(int, n, n_)
        PROPERTYPP_DECLARE(float, m, m_)
        PROPERTYPP_DECLARE(double, d, d_)
        PROPERTYPP_DECLARE(Test*, o, o_)
};

PROPERTYPP_SYNTHESIZE(Test, int, n, n_);

PROPERTYPP_SYNTHESIZE_GETTER(Test, double, d, d_);
void Test::setd(double d)
{
    if (d >= 0)
        d_ = d;
}

PROPERTYPP_SYNTHESIZE(Test, Test*, o, o_);

float Test::getm() const
{
    std::cout << "(" << this << ") Getting m: " << m_ << std::endl;
    return m_;
}

void Test::setm(float value)
{
    std::cout << "(" << this << ") Setting m to " << value << std::endl;
    m_ = value;
}

Test::Test()
{
    PROPERTYPP_INIT(n);
    PROPERTYPP_INIT(m);
    PROPERTYPP_INIT(d);
    PROPERTYPP_INIT(o);
}

Test::Test(const Test& rhs)
{
    PROPERTYPP_COPY(n, rhs);
    PROPERTYPP_COPY(m, rhs);
    PROPERTYPP_COPY(d, rhs);
    PROPERTYPP_COPY(o, rhs);
}

int main()
{
   Test a;
   assert(a.n == 0);
   assert(a.m == 0);
   assert(a.d == 0);
   assert(a.o == 0);

   a.n = 1;
   a.m = 2;
   assert(a.n == 1);
   assert(a.m == 2);

   a.n = a.m;
   assert(a.n == 2);
   assert(a.m == 2);

   a.n = a.m = 3;
   assert(a.n == 3);
   assert(a.m == 3);

   a.n = 4;
   assert(a.n == 4);
   assert(a.m == 3);

   a.m = 5;
   assert(a.n == 4);
   assert(a.m == 5);

   a.d = a.n + a.m;
   assert(a.d == 9);
   assert(a.n == 4);
   assert(a.m == 5);

   a.d = -1.0;
   assert(a.d == 9);
   assert(a.n == 4);
   assert(a.m == 5);

   Test b = a;
   assert(b.n == 4);
   assert(b.m == 5);
   assert(b.d == 9);

   b.n = 10;
   assert(b.n == 10);
   assert(b.m == 5);
   assert(b.d == 9);
   assert(a.n == 4);
   assert(a.m == 5);
   assert(a.d == 9);

   a.o = new Test;
   assert(a.o->n == 0);
   assert(a.o->m == 0);
   assert(a.o->d == 0);
   assert(a.o->o == 0);

   a.o->n = 10;
   a.o->m = 0;
   a.o->d = 0;
   a.o->o = 0;
   assert(a.n == 4);
   assert(a.m == 5);
   assert(a.d == 9);
   assert(a.o->n == 10);
   assert(a.o->m == 0);
   assert(a.o->d == 0);
   assert(a.o->o == 0);

   Test c = *(a.o);
   assert(c.n == 10);
   assert(c.m == 0);
   assert(c.d == 0);
   assert(c.o == 0);

   c.n = 11;
   c.m = 1;
   c.d = 1;
   assert(c.n == 11);
   assert(c.m == 1);
   assert(c.d == 1);
   assert(c.o == 0);
   assert(a.n == 4);
   assert(a.m == 5);
   assert(a.d == 9);
   assert(a.o->n == 10);
   assert(a.o->m == 0);
   assert(a.o->d == 0);
   assert(a.o->o == 0);

   a.n = -a.n;
   assert(a.n == -4);

   assert(a.n != a.m);
   a.n = 12;
   a.m = 12;
   assert(a.n == a.m);

   ++a.n;
   assert(a.n == 13);

   a.n++;
   assert(a.n == 14);

   --a.n;
   assert(a.n == 13);

   a.n--;
   assert(a.n == 12);

   a.n = 0;
   assert(a.n == 0);

   int v;
   v = a.n++;
   assert(v == 0);

   v = ++a.n;
   assert(v == 2);

   v = a.n--;
   assert(v == 2);

   v = --a.n;
   assert(v == 0);

   return 0;
}
