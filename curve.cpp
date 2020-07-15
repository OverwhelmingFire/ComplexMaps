#include "curve.h"

curve::curve() {
    vertices.push_back(complex(0.0,0.0));
    r=0.0;
    g=0.0;
    b=0.0;
}

curve::curve(complex const& _origin) {
    vertices.push_back(_origin);
    r=0.0;
    g=0.0;
    b=0.0;
}

void curve::resize(complex const& _end) {
    vertices.push_back(_end);
}

void curve::resize(std::vector<complex> _input, complex modifyer(complex const&)) {
    vertices.clear();

    for (int i = 0; i < _input.size(); i++) {
        vertices.push_back(modifyer(_input[i]));
    }
}

void curve::draw() {
    glBegin(GL_LINE_STRIP);
    glColor3d(r,g,b);
    for (int i = 0; i < vertices.size(); i++) {
        glVertex3d(vertices[i].re, vertices[i].im, 0);
    }
    glEnd();
}

void curve::out() {
    std::cout << "End: " << vertices[vertices.size()-1].re << "; " << vertices[vertices.size()-1].im << std::endl;
}
