#include "ellipse.h"

ellipse::ellipse() {
    origin = complex(0,0);
    end = complex(0,0);
    r=0.0;
    g=0.0;
    b=0.0;
    minor = 0.5;
}

ellipse::ellipse(complex const& _origin) {
    origin = complex(_origin.re, _origin.im);
    end = complex(0,0);
    r=0.0;
    g=0.0;
    b=0.0;
    minor = 0.5;
}

ellipse::ellipse(complex const& _origin, complex const& _end) {
    origin = _origin;
    end = _end;

    r=0.0;
    g=0.0;
    b=0.0;
    minor = 0.5;
    double major = sqrt(pow(end.re-origin.re,2)+pow(end.im-origin.im,2));

    double angle = atan2(end.im-origin.im, end.re-origin.re);
    complex rot(cos(angle), sin(angle));
    for (double degree = 0; degree <= 360; degree++) {
        complex temp(major*cos(degree*2*3.1415/360), minor*sin(degree*2*3.1415/360));
        vertices.push_back(temp*rot+origin);
    }
}

void ellipse::resize(complex const& _end) {
    vertices.clear();
    end = _end;
    double major = sqrt(pow(end.re-origin.re,2)+pow(end.im-origin.im,2));

    double angle = atan2(end.im-origin.im, end.re-origin.re);
    complex rot(cos(angle), sin(angle));
    for (double degree = 0; degree <= 360; degree++) {
        complex temp(major*cos(degree*2*3.1415/360), minor*sin(degree*2*3.1415/360));
        vertices.push_back(temp*rot+origin);
    }
}

void ellipse::resize(double scale) {
    std::cout << minor << "!!" << std::endl;
    vertices.clear();
    std::cout << minor << "!!" << std::endl;
    minor = minor*scale;
    std::cout << minor << "!!" << std::endl;
    double major = sqrt(pow(end.re-origin.re,2)+pow(end.im-origin.im,2));

    double angle = atan2(end.im-origin.im, end.re-origin.re);
    complex rot(cos(angle), sin(angle));
    for (double degree = 0; degree <= 360; degree++) {
        complex temp(major*cos(degree*2*3.1415/360), minor*sin(degree*2*3.1415/360));
        vertices.push_back(temp*rot+origin);
    }
}

void ellipse::draw() {
    glBegin(GL_LINE_STRIP);
    glColor3d(r,g,b);
    for (int i = 0; i < vertices.size(); i++) {
        glVertex3d(vertices[i].re, vertices[i].im, 0);
    }
    glEnd();
}

void ellipse::out() {
    complex _end = vertices[vertices.size()-1];
    double distance = sqrt(pow(_end.re-origin.re,2)+pow(_end.im-origin.im,2));
    double angle = atan2(_end.im-origin.im, _end.re-origin.re);
    std::cout << "Angle: " << angle*360/(2*3.1415) << " deg; Distance: " << distance << std::endl;
}