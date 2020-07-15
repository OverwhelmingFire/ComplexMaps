#ifndef ELLIPSE
#define ELLIPSE

#include "primitive.h"

class ellipse : public primitive {
    private:
        double minor;
        complex end;
    public:
        ellipse();
        ellipse(complex const& _origin);
        ellipse(complex const& _origin, complex const& end);

        virtual void resize(double _minor);
        virtual void resize(complex const& _end);
        virtual void draw();
        virtual void out();
};

#endif