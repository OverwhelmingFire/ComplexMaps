#ifndef CURVE
#define CURVE

#include "primitive.h"
#include <vector>

class curve : public primitive {
    public:
        curve();
        curve(complex const& _origin);

        virtual void resize(complex const& _end);
        virtual void resize(std::vector<complex> _input, complex modifyer(complex const&));
        virtual void draw();
        virtual void out();
};

#endif