#ifndef PRIMITIVE
#define PRIMITIVE

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include "complex.h"

class primitive {
    protected:
        double r, g, b;
        primitive() {}
        complex origin;
    public:
        std::vector<complex> const getVertices () const {
            return vertices;
        }
        virtual ~primitive() {}
        std::vector<complex> vertices;

        virtual void resize(double d) {}
        virtual void resize(complex const& _end) {}
        virtual void draw() {
            std::cout << "NO !" << std::endl;
        }
        virtual void out() {}
};

#endif