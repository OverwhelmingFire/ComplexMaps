#ifndef FUNCTION
#define FUNCTION

#include "complex.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>

class function {
    public:
        char functionType; // 0 - common func (2 operands), 1 - elementary func (1 operand), 2 - const, 3 - var
        char oper = ' ';
        function * operand1;
        function * operand2;
        complex constant;
        bool var;

        function();

        function(char o, function * o1, function * o2);

        function(char o, function * o1);

        function(complex const& con);

        void out(std::string tab);

        complex calc(complex const& z);
};

#endif