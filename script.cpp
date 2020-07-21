#ifdef FOR_WINDOWS
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl.h>
#include <glu.h>
#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <regex>
#include <string>
#include <map>
#include "function.h"
#include "complex.h"
#include "primitive.h"
#include "line.h"
#include "curve.h"
#include "ellipse.h"
#include "text.h"
#include <chrono>
#endif

#ifndef FOR_WINDOWS
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <regex>
#include <string>
#include <map>
#include "function.h"
#include "complex.h"
#include "primitive.h"
#include "line.h"
#include "curve.h"
#include "ellipse.h"
#include "text.h"
#include <chrono>
#endif

// @todo: get rid of glVertex in the entire project, make use of glDrawArray (it will speed up the drawing process)

/*

Int MODE defines a type of primitive which you can paint with your mouse..
There are several modes: curve, straight line and ellipse.
Modes are switched by pressing 1, 2 or 3 on the keyboard.

*/

#define MODE_CURVE 1
#define MODE_LINE 2
#define MODE_ELLIPSE 3
int MODE = MODE_CURVE;

/*

Donw there goes every stuff concerned with layout. Everything is measured in pixels.

In the program 3 different canvas are drawn.
1. Canvas: this is a canvas with PRE image, on which you can draw curves, lines etc.
2. ImageCanvas: this is a canvas with IMAGE, on which an IMAGE of the curves and lines is painted
3. MenuCanvas: this is a canvas with textboxes, it currently consist of two distinct textboxes: one for displaying text and another one for entering text

Span is the distance from any canvas to the nearest edge of window.

*/

int windowWidth=700;
int windowHeight=300;

int xCanvas;
int wCanvas;
int hCanvas;
int yCanvas;

int xImageCanvas;
int wImageCanvas;
int hImageCanvas;
int yImageCanvas;

int xMenuCanvas;
int wMenuCanvas;
int hMenuCanvas;
int yMenuCanvas;

int span=20;

/*

You can move contents of canvas using spacebar+mouse. Moreover, you can zoom it using mousewheel.

dx, dy: amount of pixels on which contents of the 1 and 2 canvas are moved
scale: measure of zoom

prevX, prevY: it stores previous position of the mouse; it changes every time you move your mouse
begin: it is set to the coordinates of the point where you pressed Left Mouse Button; it is set to 0 every time you release Left Mouse Button
f: our function which coverts the pre-images into the images. For example, it converts:
    - preimage => image
    - _precurve => _curve
    - hlines, vlines => hlinesMap, vlinesMap
    - haxe, vaxe => haxeMap, vaxeMap

*/

double dx=0;
double dy=0;

double RESOLUTION = 200;
double scale=1;

bool spacePressed = false;
bool shift = false;

int prevX = 0;
int prevY = 0;
complex * begin = 0;
function * f = new function();

/*

preimage: a vector of original primitives
_precurve: temporary vector used to store a primitive which is not finished
image: a vector of mapped primitives, or curves
_curve: temporary vector used to store a mapped primitive which is not finished
hlines, vlines: vector of horizontal and vertical lines of the graph
haxe, vaxe: axes of the graph
hlinesMap, vlinesMap: vector of maps of horizontal and vertical lines of the graph
haxeMap, vaxeMap: map of axes

*/

std::vector<primitive*> preimage;
primitive* _precurve;
std::vector<curve*> image;
curve* _curve;
std::vector<std::vector<complex>> hlines;
std::vector<std::vector<complex>> vlines;
std::vector<complex> haxe;
std::vector<complex> vaxe;
std::vector<std::vector<complex>> hlinesMap;
std::vector<std::vector<complex>> vlinesMap;
std::vector<complex> haxeMap;
std::vector<complex> vaxeMap;

/*

textOnCanvas: it is a text box used to display information
textEditOnCanvas: it is a text box used to edit text

*/

text textOnCanvas(wMenuCanvas,hMenuCanvas/2,xMenuCanvas,yMenuCanvas,"Please enter your function below:");
text textEditOnCanvas(wMenuCanvas,hMenuCanvas/2,xMenuCanvas,yMenuCanvas+hMenuCanvas/2,"");

/*

priority: dictionary which stores the priorty of every arithmetical operation, for example, "*" has more priority than "+" (2 > 1)
equals: dictionary which substitutes synonymous functions with abbreviation, for example, "ln" and "log" are synonymous and replaced with "l"

*/

std::map<char, int> priority = {
    {'^',3},
    {'*',2},
    {'/',2},
    {'+',1},
    {'-',1}
};

std::map<std::string, char> equals = {
    {"ln", 'l'},
    {"log", 'l'},
    {"l", 'l'},
    {"exp", 'e'},
    {"sin", 's'},
    {"cos", 'c'}
};

/*

Following block of functions is used to parse a string and covert it into function object

*/

bool isargument(std::string s) {
    return !s.compare("x");
}

bool iscomplex(std::string s) {
    int signcounter = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i]=='+' || s[i]=='-') {
            signcounter++;
            if (signcounter > 2) return false;
        }
        else if (!(std::isdigit(s[i]) || s[i]=='i')) return false;
    }
    return true;
}

function * parser(std::string s) {
    std::vector<std::string> operands;
    std::vector<int> isfunction;
    std::vector<char> operators;
    int lastOperatorPos = -1;
    int lOpenCounter = 0;
    int lPos = -1;
    function * ret = nullptr;
    std::cout << 111 << std::endl;
    for (int i = 0; i < s.length(); i++) {
        std::cout << 112 << std::endl;
        int value = priority[s[i]];
        if (value) {
            if (!lOpenCounter) {
                operators.push_back(s[i]);
                if (lastOperatorPos+1 != lPos) {
                    isfunction.push_back(lPos-lastOperatorPos-1);
                    operands.push_back(s.substr(lastOperatorPos+1, i-lastOperatorPos-1));
                }
                else {
                    isfunction.push_back(false);
                    operands.push_back(s.substr(lastOperatorPos+2, i-lastOperatorPos-3));
                }
                lastOperatorPos = i;
            }
        }
        else if (s[i]=='(') {
            if (!lOpenCounter) lPos = i;
            lOpenCounter++;
        }
        else if (s[i]==')') {
            lOpenCounter--;
            if (lOpenCounter<0) throw "Syntax error: excess closing bracket";
        }
        std::cout << 113 << std::endl;
    }

    std::cout << 114 << std::endl;
    if (!lOpenCounter) {
        if (lastOperatorPos+1 != lPos) {
            isfunction.push_back(lPos-lastOperatorPos-1);
            operands.push_back(s.substr(lastOperatorPos+1, s.length()-lastOperatorPos-1));
        }
        else {
            isfunction.push_back(false);
            operands.push_back(s.substr(lastOperatorPos+2, s.length()-lastOperatorPos-3));
        }
    }
    //else throw "Syntax error: no closing bracket found";

    std::cout << 115 << std::endl;

    std::cout << 116 << std::endl;
    std::vector<function*> foos(operands.size(), nullptr);
    for (int i = 0; i < operands.size(); i++) {
        std::cout << operands[i] << std::endl;
        function * f;
        if (isargument(operands[i])) {
            std::cout << 116.1 << std::endl;
            f = new function();
        }
        else if (iscomplex(operands[i])) {
            std::cout << 116.2 << std::endl;
            if (operands[i].length()==1 && operands[i][0]!='i' && !std::isdigit(operands[i][0])) {
                throw "Unknown operand";
            }
            complex constant(operands[i]);
            f = new function(constant);
        }
        else if (isfunction[i]) {
            std::cout << 116.3 << std::endl;
            if (operands[i].length()==1 && !std::isdigit(operands[i][0])) {
                throw "Unknown operand";
            }
            std::cout << 116.4 << " " << isfunction[i] << " " << operands[i].length()-isfunction[i] << std::endl;
            std::string subst = operands[i].substr(isfunction[i], operands[i].length()-isfunction[i]); // @todo: handle cases when isfunction[i] < 0
            std::cout << 116.41 << subst << std::endl;

            function * arg = parser(subst);
            std::string subst2 = operands[i].substr(0,isfunction[i]);
            std::cout << 116.5 << std::endl;
            f = new function(equals[subst2], arg); // @todo: what if there is no match in the "equals"?
            std::cout << 116.6 << std::endl;
        }
        else {
            std::cout << 116.4 << std::endl;
            f = parser(operands[i]);
        }

        foos[i] = f;
    }
    std::cout << 117 << std::endl;

    if (operands.size() == 0) throw "there are no operands";
    int i = 0;
    while (operators.size() > 0) {
        std::cout << 118 << std::endl;
        bool greaterThanLeft = false;
        bool greaterThanRight = false;
        if ((i>0 && priority[operators[i]]>=priority[operators[i-1]]) || (i == 0)) greaterThanLeft = true;
        if ((i<operators.size()-1 && priority[operators[i]]>=priority[operators[i+1]]) || (i == operators.size()-1)) greaterThanRight = true;
        if (greaterThanRight && greaterThanLeft) {
            function * foo = new function(operators[i], foos[i], foos[i+1]);
            foos.erase(foos.begin() + i);
            operators.erase(operators.begin() + i);
            foos[i] = foo;
        }

        i++;
        if (i>=operators.size()) i = 0;
    }
    std::cout << "117.1" << std::endl;
    foos[0]->out("");
    ret = foos[0];
    std::cout << "117.2" << std::endl;
    return ret;
}

/*

Following function is just a wrap of the "f"; @todo: remove func from the code

*/

complex func(complex const& z) {
    complex z1(z.re, z.im);
    complex z2(1,0);
    z1=f->calc(z);
    z1.r=z.r;
    z1.g=z.g;
    z1.b=z.b;
    return z1;
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(xCanvas, yCanvas, wCanvas, hCanvas);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glBegin(GL_LINE_STRIP);
    glColor3d(1,0,0);
    glVertex3d(-1,-1,0);
    glVertex3d(-1,1,0);
    glVertex3d(1,1,0);
    glVertex3d(1,-1,0);
    glVertex3d(-1,-1,0);
    glEnd();

    glLoadIdentity();
    glScalef(scale,scale,1);
    glTranslatef(dx,dy,0);

    GLfloat quadVertices[] = { -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f,-1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f
    };

    glVertexPointer(3, GL_FLOAT, 0, quadVertices);
    glDrawArrays(GL_QUADS, 0, 4);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < haxe.size(); i++) {
        glColor3d(haxe[i].r,haxe[i].g,haxe[i].b);
        glVertex3d(haxe[i].re,haxe[i].im,0);
    }
    glEnd();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < vaxe.size(); i++) {
        glColor3d(vaxe[i].r,vaxe[i].g,vaxe[i].b);
        glVertex3d(vaxe[i].re,vaxe[i].im,0);
    }
    glEnd();

    for (int i = 0; i < preimage.size(); i++) {
        preimage[i]->draw();
    }

    if (_precurve != nullptr) _precurve->draw();

    for (int i = 0; i < hlines.size(); i++) {
        glBegin(GL_LINE_STRIP);
        for (int k = 0; k < hlines[i].size(); k++) {
            glColor3d(0.7,0.7,0.7);
            glVertex3d(hlines[i][k].re,hlines[i][k].im, 0);
        }
        glEnd();
    }
    for (int i = 0; i < vlines.size(); i++) {
        glBegin(GL_LINE_STRIP);
        for (int k = 0; k < vlines[i].size(); k++) {
            glColor3d(0.7,0.7,0.7);
            glVertex3d(vlines[i][k].re,vlines[i][k].im, 0);
        }
        glEnd();
    }

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex3d(-1,-1,0);
    glVertex3d(-1,1,0);
    glVertex3d(1,1,0);
    glVertex3d(1,-1,0);
    glEnd();



    //glMatrixMode(GL_PROJECTION);
    glViewport(xImageCanvas, yImageCanvas, wImageCanvas, hImageCanvas);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glBegin(GL_LINE_STRIP);
    glColor3d(1,0,0);
    glVertex3d(-1,-1,0);
    glVertex3d(-1,1,0);
    glVertex3d(1,1,0);
    glVertex3d(1,-1,0);
    glVertex3d(-1,-1,0);
    glEnd();

    glLoadIdentity();
    glScalef(scale,scale,1);
    glTranslatef(dx, dy, 0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < haxeMap.size(); i++) {
        glColor3d(haxeMap[i].r,haxeMap[i].g,haxeMap[i].b);
        glVertex3d(haxeMap[i].re,haxeMap[i].im,0);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < vaxeMap.size(); i++) {
        glColor3d(vaxeMap[i].r,vaxeMap[i].g,vaxeMap[i].b);
        glVertex3d(vaxeMap[i].re,vaxeMap[i].im,0);
    }
    glEnd();

    for (int i = 0; i < image.size(); i++) {
        image[i]->draw();
    }
    if (_curve != nullptr) _curve->draw();


    for (int i = 0; i < hlinesMap.size(); i++) {
        glBegin(GL_LINE_STRIP);
        for (int k = 0; k < hlinesMap[i].size(); k++) {
            glColor3d(0.7,0.7,0.7);
            glVertex3d(hlinesMap[i][k].re,hlinesMap[i][k].im, 0);
        }
        glEnd();
    }
    for (int i = 0; i < vlinesMap.size(); i++) {
        glBegin(GL_LINE_STRIP);
        for (int k = 0; k < vlinesMap[i].size(); k++) {
            glColor3d(0.7,0.7,0.7);
            glVertex3d(vlinesMap[i][k].re,vlinesMap[i][k].im, 0);
        }
        glEnd();
    }

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glVertex3d(-10,-10,0);
    glVertex3d(-10,10,0);
    glVertex3d(10,10,0);
    glVertex3d(10,-10,0);
    glEnd();



    textOnCanvas.draw();
    textEditOnCanvas.draw();
    glFlush();
    glutSwapBuffers();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "[ns]" << std::endl;
}

void update(int value)
{
        glutPostRedisplay();
        glutTimerFunc(0.1, update, 0);
}

void initRendering()
{
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
}

void handleResize(int w, int h)
{
        glViewport(0, 0, w, h);
        windowWidth=w;
        windowHeight=h;

        int wPanel = (windowWidth-4*span)/3;
        int hPanel = windowHeight-2*span;

        int t = 0;
        if (wPanel<hPanel) {
            wCanvas=wPanel;
            hCanvas=wPanel;
            xCanvas=span;
            yCanvas=(windowHeight-hCanvas)/2;

            wImageCanvas=wPanel;
            hImageCanvas=wPanel;
            xImageCanvas=2*span+wCanvas;
            yImageCanvas=(windowHeight-hCanvas)/2;

            wMenuCanvas=wPanel;
            hMenuCanvas=wPanel;
            xMenuCanvas=3*span+wCanvas+wImageCanvas;
            yMenuCanvas=(windowHeight-hCanvas)/2;
        }
        else {
            int t = (windowWidth-4*span-3*hPanel)/6;
            wCanvas=hPanel;
            hCanvas=hPanel;
            xCanvas=span+t;
            yCanvas=span;

            wImageCanvas=hPanel;
            hImageCanvas=hPanel;
            xImageCanvas=2*span+wCanvas+3*t;
            yImageCanvas=span;

            wMenuCanvas=hPanel;
            hMenuCanvas=hPanel;
            xMenuCanvas=3*span+wCanvas+wImageCanvas+5*t;
            yMenuCanvas=span;
        }

        textOnCanvas.resize(wMenuCanvas, hMenuCanvas/2, xMenuCanvas, yMenuCanvas);
        textEditOnCanvas.resize(wMenuCanvas, hMenuCanvas/2, xMenuCanvas, yMenuCanvas+hMenuCanvas/2);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

/*

Now we create event handlers, for example, to handle mouse clicks, interaction with keyboard etc.

*/

void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            {
                if(state == GLUT_UP)
                {
                    if (_precurve != nullptr && _curve != nullptr) {
                        preimage.push_back(_precurve);
                        image.push_back(_curve);
                        _precurve = nullptr;
                        _curve = nullptr;
                        if (begin)  {
                            delete begin;
                            begin = 0;
                        }
                    }
                }
                else
                {
                    textEditOnCanvas.inside(x,y);
                }
            break;
        }

        case GLUT_RIGHT_BUTTON:
            {
                if(state == GLUT_UP) {
                }
                else {
                }
            break;
        }

        case 3: // SCROLLING DOWN
            {
                if(state == GLUT_UP) {
                    if (MODE == MODE_ELLIPSE && _precurve != nullptr) {
                        _precurve->resize(1.2);
                    }
                    else {
                        scale=scale*1.2;
                    }
                }
            break;
        }

        case 4: // SCROLLING UP
            {
                if(state == GLUT_UP) {
                    if (MODE == MODE_ELLIPSE && _precurve != nullptr) {
                        _precurve->resize(1/(1.2));
                    }
                    else {
                        scale=scale/1.2;
                    }
                }
            break;
        }
    }
}

void init() {
    hlines.clear();
    vlines.clear();
    haxe.clear();
    vaxe.clear();
    hlinesMap.clear();
    vlinesMap.clear();
    haxeMap.clear();
    vaxeMap.clear();
    /*
        The code below fills with contents the following variables:

        hlines
        vlines
        haxe
        vaxe
        haxeMap
        vaxeMap

        Amount of points in every line of the graph depends on the RESOLUTION
    */

    for (int r = 0; r <= RESOLUTION; r++) {
        std::vector<complex> temph;
        std::vector<complex> tempv;
        for (int c = 0; c <= RESOLUTION; c++) {
            temph.push_back(complex(((double)r/(double)RESOLUTION-0.5)*2, ((double)c/(double)RESOLUTION-0.5)*2));
        }
        for (int c = 0; c <= RESOLUTION; c++) {
            tempv.push_back(complex(((double)c/(double)RESOLUTION-0.5)*2, ((double)r/(double)RESOLUTION-0.5)*2));
        }
        hlines.push_back(temph);
        vlines.push_back(tempv);

        haxe.push_back(complex(((double)r/(double)RESOLUTION-0.5)*2, 0, 1-(double)r/(double)RESOLUTION, (double)r/(double)RESOLUTION, 0));
        vaxe.push_back(complex(0, ((double)r/(double)RESOLUTION-0.5)*2, (double)r/(double)RESOLUTION-0.2, (double)r/(double)RESOLUTION-0.2, 1-(double)r/(double)RESOLUTION));

        haxeMap.push_back(func(haxe[haxe.size()-1]));
        vaxeMap.push_back(func(vaxe[vaxe.size()-1]));
    }

    /*
        hlinesMap
        vlinesMap
    */

    for (int i = 0; i < hlines.size(); i++) {
        std::vector<complex> temp;
        for (int k = 0; k < hlines[i].size(); k++) {
            temp.push_back(func(hlines[i][k]));
        }
        hlinesMap.push_back(temp);
    }

    for (int i = 0; i < vlines.size(); i++) {
        std::vector<complex> temp;
        for (int k = 0; k < vlines[i].size(); k++) {
            temp.push_back(func(vlines[i][k]));
        }
        vlinesMap.push_back(temp);
    }

    /*

    Re-filling the vector with mapped curves

    */

    image.clear();
    for (int i = 0; i < preimage.size(); i++) {
        curve * tc = new curve();
        (*tc) = (*f)(*(preimage[i]));
        image.push_back(tc);
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (textEditOnCanvas.issetcursor()) {
        textEditOnCanvas.keyboard(key);
        function * tf = nullptr;
        try {
            tf = parser(textEditOnCanvas.gettext());
        }
        catch (const char * e) {
            std::cout << "Error: " << e << std::endl;
        }
        catch (const std::out_of_range& e) {
            //std::cout << "Error: " << e << std::endl;
        }
        catch (const std::invalid_argument& e) {
            //std::cout << "Error: " << e << std::endl;
        }
        if (tf == nullptr) std::cout << "No function has been generated." << std::endl;
        else {
            f = tf;
            init();
        }
    }
    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        shift = true;
    }
    else {
        shift = false;
    }
    if ((glutGetModifiers() == GLUT_ACTIVE_CTRL) && (key+96 == 'z')) {
        if ((_precurve == nullptr) && !(preimage.empty())) {
            preimage.pop_back();
            image.pop_back();
        }
        else {
            delete _precurve;
            _precurve = nullptr;

            delete _curve;
            _curve = nullptr;
        }
    }
    if (key == ' ') {
        spacePressed=true;
    }                       // REFACTOR TO SWITCH
    else if (key <= '9' && key >= '0') {
        MODE = key-'0';
    }
}


void special(int key, int x, int y) {
    textEditOnCanvas.special(key);
    if (key == 112 || key == 113) {
        shift = true;
    }
    else {
        shift = false;
    }
}


void mouseMoving(int x, int y) {
    double _x = (double)x;
    double _y = windowHeight-(double)y;
    double re;
    double im;

    re = (double)(x-(xCanvas+dx*scale*wCanvas/2+wCanvas/2))/(double)(scale*wCanvas/2);
    im = (double)(_y-(yCanvas+dy*scale*hCanvas/2+hCanvas/2))/(double)(scale*hCanvas/2);
    complex c(re, im);
    complex temp = func(c);
    if (_precurve!=nullptr || begin != 0) {
        if (shift) {
            _precurve->resize(c);
            _curve->resize(_precurve->getVertices(),func);
        }
        
        else {
            _precurve->resize(c);
            _curve->resize(_precurve->getVertices(),func);
        }
    }
    else {
        switch (MODE) {
            case MODE_CURVE : {
                _precurve = new curve(c);
                _curve = new curve(temp);
                break;
            }
            case MODE_LINE : {
                _precurve = new line(c);
                _curve = new curve(temp);
                break;
            }
            case MODE_ELLIPSE : {
                _precurve = new ellipse(c);
                _curve = new curve(temp);
                break;
            }
            default: {
                break;
            }
        }
    }
}

void mousePassive(int x, int y) {
    if (spacePressed) {
        dx+=(double)(x-prevX)*2/(scale*wCanvas);
        dy+=(double)(prevY-y)*2/(scale*hCanvas);
    }
    prevX = x;
    prevY = y;
}

void keyboardUp(unsigned char key, int x, int y) {
    if (key == ' ') {
        spacePressed=false;
        prevX = x;
        prevY = y;
    }
}

void specialUp(int key, int x, int y) {
        if (key == 112 || key == 113) {
            shift = false;
        }
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Complex Maps");
    initRendering();

    glutDisplayFunc(draw);
    glutReshapeFunc(handleResize);
    glutSpecialFunc(special);
    glutTimerFunc(25, update, 0);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialUpFunc(specialUp);
    glutMotionFunc(mouseMoving);
    glutPassiveMotionFunc(mousePassive);

    std::string expression = "exp(x)";
    if (argc>1) {
        expression=argv[1];
    }
    try {
        f = parser(expression);
    }
    catch (const char * e) {
        std::cout << "Error: " << e << std::endl;
    }
    catch (const std::out_of_range& e) {
        //std::cout << "Error: " << e << std::endl;
    }
    catch (const std::invalid_argument& e) {
        //std::cout << "Error: " << e << std::endl;
    }
    f->out("");
    init();

    glutMainLoop();
    return 0;
}