# About #

propertypp adds Objective-C-like properties to C++ classes. Reading and
writing properties uses the same syntax as reading and writing public
member variables, but getters and setters get called behind the scenes. For
example, an `int` property `p` of an object `o` can be read like this:

```c++
int v = o.p;
```

But what actually happens is this:

```c++
int v = o.getp();
```

Likewise, writing to `p` uses the following syntax:

```c++
o.p = 5;
```

But what actually takes place is:

```c++
o.setp(5);
```

Keep reading for instructions of how to use propertypp in your projects.

# Install #

propertypp comes in a single header file, `propertypp.h`. Just copy it
into your code base.

# Example code #

```c++
#include <iostream>
#include "propertypp.h"

class Example
{
    public:
        Example();
        PROPERTYPP_DECLARE(int, n, n_) // declare property n of type int
};

Example::Example()
{
    PROPERTYPP_INIT(n); // initialize property n
}

PROPERTYPP_SYNTHESIZE_GETTER(Example, int, n, n_) // synthesize getter

void setn(int value)
{
    if (value >= 0)
        n_ = value;
}

int main()
{
    Example e;

    std::cout << e.n << std::endl; // should print 0

    e.n = 5;
    std::cout << e.n << std::endl; // should print 5

    e.n = -1;
    std::cout << e.n << std::endl; // should print 5

    return 0;
}
```

# Use #

propertypp uses a number of macros to add and configure properties in
C++ classes.

## Declaring properties ##

To declare a property for a class, use the `PROPERTYPP_DECLARE` macro.

```c++
class Example
{
    public:
        Example();
        PROPERTYPP_DECLARE(int, n, n_)
};
```

The arguments to `PROPERTYPP_DECLARE` are the type of the property (`int` in
the example), the name of the property (`n` in the example) and the name of the
member variable that will hold the value of the property (`n_` in the example).
Notice that you don't have to declare the member variable, because the
`PROPERTYPP_DECLARE` macro does that for you.

*IMPORTANT*: properties are always public, even if you call the
`PROPERTYPP_DECLARE` macro in `private:` or `protected:` scope.

*IMPORTANT*: after you declare a property, you are left in `public:` scope.

## Defining properties ##

The `PROPERTYPP_DECLARE` macro only declares property-related
functionality for the class. In order to make properties work, their
accessor methods i.e. their getters and setters must be defined.

There are two options for defining property accessor methods.
One is to use the `PROPERTYPP_SYNTHESIZE`* macros to generate them.
You can also write your own accessor methods.

### Synthesizing getters and setters ###

The macro `PROPERTYPP_SYNTHESIZE` generates a default getter and setter for
a property:

```c++
PROPERTYPP_SYNTHESIZE(Example, int, n, n_)
```

The arguments to `PROPERTYPP_SYNTHESIZE` are the name of the class that
contains the property, the type of the property, the name of the property
and the name of the member variable that stores the value of the property.

If you only want to synthesize the getter or the setter and write the other
accessor method yourself, you can use the `PROPERTYPP_SYNTHESIZE_GETTER` or
`PROPERTYPP_SYNTHESIZE_SETTER` macros, respectively. The arguments to those
are exactly the same `PROPERTYPP_SYNTHESIZE` takes.

*IMPORTANT*: don't call the `PROPERTYPP_SYTHESIZE`* macros inside a class
declaration.

### Writing your own accessor methods ###

If you want to write the code for an accessor method your self, you have
to write the getter and/or setter with the exact following signatures:

Getter:

```c++
type get<name>() const;
```

Setter:

```c++
void set<name>(type) const;
```

Where name is the name of the property as declared with the
`PROPERTYPP_DECLARE` macro and type is the type of the property. For example,
the accessor methods for the `n` property in the `Example` class above
could be written as follows:

```c++
int getn() const
{
    return n_;
}

void setn(int val)
{
    n_ = val;
}
```

Notice that the property name appears as-is on the function names i.e. no
camel case. If you define accessor with a signature different than that
presented, your code will not build.

## Initializing properties ###

Properties must be initialized with the `PROPERTYPP_INIT` macro before they
can be used.

```c++
Example::Example()
{
    PROPERTYPP_INIT(n);
}
```

The `PROPERTYPP_INIT` macro takes a sole argument, which is the name of the
property. Property values are always initialized to 0.

*IMPORTANT*: the constructor of a class is likely the best place to initialize
any properties it has, but that is not required. However, using a property
before it is initialized will probably cause your program to crash.

## Using properties ##

As explained in the About section, properties can be read and written just
as if they were a member variable of their class. For example:

```c++
Example e;
e.n = 12;
int v = e.n; // v should be 12
```

## Copying properties ##

If a class containing properties is to support copying via a copy constructor
and/or an assignment operator, those methods must be written i.e. you can't
rely on compiler-generated copy constructors and assignment operators for
proper property copying.

To copy a property, use the `PROPERTYPP_COPY` macro:

```c++
Example::Example(const Example& rhs)
{
    PROPERTYPP_COPY(n, rhs);
}
```

The arguments to `PROPERTYPP_COPY` are the name of the property and the
name of the variable where the object containing the property is copying
its values from.

Failing to copy properties with the `PROPERTYPP_COPY` macro will lead to
property-sharing behavior i.e. setting a property on an object created by
copying another will cause that property to be set on the original object also.

# Drawbacks #

* propertypp currently only works with built-in and pointer property types.
* The behind-the-scenes calls to getters and setters have the cost of
  virtual function calls.

# License #

propertypp - Objective-C-like properties for C++ classes.

Copyright (c) 2011 Cesar L. B. Silveira

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
