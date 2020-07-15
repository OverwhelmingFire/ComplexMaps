#include "function.h"


function::function(char o, function * o1, function * o2) {
    functionType = 0;
    oper = o;
    operand1 = o1;
    operand2 = o2;
    var = false;
};

function::function(char o, function * o1) {
    functionType = 1;
    oper = o;
    operand1 = o1;
    operand2 = 0;
    var = false;
};

function::function(complex const& con) {
    functionType = 2;
    constant = con;
    operand1 = 0;
    operand2 = 0;
    oper = 0;
    var = false;
};

function::function() {
    functionType = 3;
    oper = 0;
    operand1 = 0;
    operand2 = 0;
    var = true;
};

void function::out(std::string tab) {
    tab+="|-";
    switch (functionType) {
        case 0 : {
            std::cout << tab << oper << std::endl;
            operand1->out(tab+"\t");
            operand2->out(tab+"\t");
            break;
        }
        case 1 : {
            std::cout << tab << oper << std::endl;
            operand1->out(tab+"\t");
            break;
        }
        case 2 : {
            std::cout << tab << constant << std::endl;
            break;
        }
        case 3 : {
            std::cout << tab << "x" << std::endl;
            break;
        }
    }
}

complex function::calc(complex const& z) {
    if (functionType == 2) {
        return constant;
    }
    else if (functionType == 3) {
        return z;
    }
    else {
        switch (oper) {
            case '+': {
                return (operand1->calc(z))+(operand2->calc(z));
                break;
            }
            case '-': {
                return (operand1->calc(z))-(operand2->calc(z));
                break;
            }
            case '*': {
                return (operand1->calc(z))*(operand2->calc(z));
                break;
            }
            case '/': {
                return (operand1->calc(z))/(operand2->calc(z));
                break;
            }
            case '^': {
                return (operand1->calc(z))^(operand2->calc(z));
                break;
            }
            case 'l': {
                return (operand1->calc(z)).ln();
            }
            case 'e': {
                return (operand1->calc(z)).ex();
            }
            case 's': {
                return (operand1->calc(z)).sn();
            }
            case 'c': {
                return (operand1->calc(z)).cs();
            }
            default: {
                break;
            }
         }
    }
};
