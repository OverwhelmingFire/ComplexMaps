#include "complex.h"

complex::complex() {
    re=0;
    im=0;
    r=0;
    g=0;
    b=0;
}

complex::complex(double x, double y) {
    re=x;
    im=y;
    r=0;
    g=0;
    b=0;
}

complex::complex(double x) {
    re=x;
    im=0;
    r=0;
    g=0;
    b=0;
}

complex::complex (const complex &z) {
    re=z.re;
    im=z.im;
    r=z.r;
    g=z.g;
    b=z.b;
}

complex::complex (const complex &z, double _r, double _g, double _b) {
    re=z.re;
    im=z.im;
    r=_r;
    g=_g;
    b=_b;
}

complex::complex(double x, double y, double _r, double _g, double _b) {
    re=x;
    im=y;
    r=_r;
    g=_g;
    b=_b;
}

complex::complex(std::string s) {
    bool hasimaginary = false;
    int signcounter = 0;
    int lastsign = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i]=='+' || s[i]=='-') {
            signcounter++;
            lastsign = i;
        }
        else if (s[i]=='i') {
            if (i<s.length()-1) {
                throw "Wrong format of creating complex number";
            }
            else {
                hasimaginary = true;
            }
        }
    }
    if (signcounter > 2) throw "Wrong format of creating complex number";
    else {
        if (lastsign > 0) {
            if (hasimaginary) {
                re = std::stod(s.substr(0,lastsign));
                std::string strim = s.substr(lastsign,s.length()-lastsign-1);
                //std::cout << "asd " << strim+"1";
                strim = strim.length() > 1 ? strim : strim+"1";
                //std::cout << "asd " << strim;
                im = std::stod(strim);
            }
            else {
                throw "Complex number has more than 1 real part";
            }
        }
        else {
            if (hasimaginary) {
                //std::cout << 43534535 << std::endl;
                re = 0;
                std::string strim = s.substr(0,s.length()-1);
                //std::cout << "asd " << strim << std::endl;
                strim = strim.length() > 1 ? strim : strim+"1";
                //std::cout << "asdsadfd "<< strim << std::endl;
                im = std::stod(strim);
            }
            else {
                re = std::stod(s);
                im = 0;
            }
        }
    }
}

double complex::mod() {
    double res = sqrt(pow(this->im,2)+pow(this->re,2));
    return res;
}

double complex::arg() {
    double res = atan2(this->im,this->re);
    return res;
}

complex complex::ln() {
    complex res(log(sqrt(pow(this->im,2)+pow(this->re,2))),atan2(this->im,this->re));
    return res;
}

complex complex::ex() {
    complex res(exp(this->re)*cos(this->im), exp(this->re)*sin(this->im));
    return res;
}

complex complex::sn() {
    complex res(sin(re)*cosh(im),cos(re)*sinh(im));
    //std::cout << sin(re)*cosh(im) << "+i*" << cos(re)*sinh(im) << std::endl;
    return res;
}

complex complex::cs() {
    complex res(cos(re)*cosh(im),-sin(re)*sinh(im));
    return res;
}

complex complex::operator + (const complex &z2) {
    complex res(this->re+z2.re, this->im+z2.im);
    return res;
}

complex complex::operator - (const complex &z2) {
    complex res(this->re-z2.re, this->im-z2.im);
    return res;
}

complex complex::operator * (const complex &z2) {
    complex res(this->re*z2.re-this->im*z2.im, this->re*z2.im+this->im*z2.re);
    return res;
}

complex complex::operator / (const complex &z2) {
    complex res((this->re*z2.re+this->im*z2.im)/(z2.re*z2.re+z2.im*z2.im), (z2.re*this->im-this->re*z2.im)/(z2.re*z2.re+z2.im*z2.im));
    return res;
}

complex complex::operator ^ (const complex &a) {
    complex res(pow(this->mod(),a.re*cos(a.re*this->arg())), pow(this->mod(),a.re*sin(a.re*this->arg())));
    return res;
}

std::ostream &operator<<(std::ostream &os, complex const &z) {
        return os << z.re << "+" << z.im << "i";
}

