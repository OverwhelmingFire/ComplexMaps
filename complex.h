#ifndef COMPLEX
#define COMPLEX



#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>

class complex {
public:
    double re;
    double im;
    double r;
    double g;
    double b;

    complex(double x, double y);

    complex();

    complex(double x);

    complex (const complex &z);

    complex (const complex &z, double _r, double _g, double _b);

    complex(double x, double y, double _r, double _g, double _b);

    complex(std::string s);

    double mod();

    double arg();

    complex ln();

    complex ex();

    complex sn();

    complex cs();

    complex operator + (const complex &z2);

    complex operator - (const complex &z2);

    complex operator * (const complex &z2);

    complex operator / (const complex &z2);

    complex operator ^ (const complex &a);
};

std::ostream &operator<<(std::ostream &os, complex const &z);

#endif