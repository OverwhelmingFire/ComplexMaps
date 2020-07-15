#ifndef LINE
#define LINE

#include "primitive.h"

class line : public primitive {
    public:
        line();
        line(complex const& _origin);
        line(complex const& _origin, complex const& end);

        virtual void resize(complex const& _end);
        virtual void draw();
        virtual void out();
};

#endif