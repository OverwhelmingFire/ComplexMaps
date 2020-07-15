#include "line.h"

line::line() {
    origin = complex(0,0);
    r=0.0;
    g=0.0;
    b=0.0;
}

line::line(complex const& _origin) {
    origin = complex(_origin.re, _origin.im);
    r=0.0;
    g=0.0;
    b=0.0;
}

line::line(complex const& _origin, complex const& _end) {
    origin = complex(_origin.re, _origin.im);

    double distance = sqrt(pow(_end.re-_origin.re,2)+pow(_end.im-_origin.im,2));
    double angle = atan2(_end.im-origin.im, _end.re-origin.re);
    for (double i = 0; i < distance; i+=distance/50) {
        vertices.push_back(complex(_origin.re + i*cos(angle), _origin.im + i*sin(angle)));
    }
    r=0.0;
    g=0.0;
    b=0.0;
}

void line::resize(complex const& _end) {
    vertices.clear();

    double distance = sqrt(pow(_end.re-origin.re,2)+pow(_end.im-origin.im,2));
    double angle = atan2(_end.im-origin.im, _end.re-origin.re);
    for (double i = 0; i < distance; i+=distance/50) {
        vertices.push_back(complex(origin.re + i*cos(angle), origin.im + i*sin(angle)));
    }
}

void line::draw() {
    glBegin(GL_LINE_STRIP);
    glColor3d(r,g,b);
    for (int i = 0; i < vertices.size(); i++) {
        glVertex3d(vertices[i].re, vertices[i].im, 0);
    }
    glEnd();
}

void line::out() {
    complex _end = vertices[vertices.size()-1];
    double distance = sqrt(pow(_end.re-origin.re,2)+pow(_end.im-origin.im,2));
    double angle = atan2(_end.im-origin.im, _end.re-origin.re);
    std::cout << "Angle: " << angle*360/(2*3.1415) << " deg; Distance: " << distance << std::endl;
}