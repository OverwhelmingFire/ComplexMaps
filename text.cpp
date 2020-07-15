#include "text.h"

text::text() {
    w = 0;
    h = 0;
    s = "";
    bound = true;
    font = GLUT_BITMAP_HELVETICA_18;
}

text::text(int const _w, int const _h) {
    w = _w;
    h = _h;
    s = "";
    bound = true;
    font = GLUT_BITMAP_HELVETICA_18;
}

text::text(int const _w, int const _h, int _x, int _y, std::string _s) {
    w = _w;
    h = _h;
    x = _x;
    y = _y;
    s = _s;
std::cout << "y " << y << std::endl;
    bound = true;
    font = GLUT_BITMAP_HELVETICA_18;

    generateWords();
}

void text::generateWords() {
    words.clear();
    int _lastPos = 0;
    for (int i = 1; i < s.length(); i++) {
        if ((s[i]==' ' && s[i-1]!=' ') || (s[i]!=' ' && s[i-1]==' ')) {
            words.push_back(s.substr(_lastPos, i-_lastPos));
            _lastPos = i;
        }
    }
    words.push_back(s.substr(_lastPos, s.length()-_lastPos));
}

void text::generateStringsBuf() {
    stringsBuf.clear();
    std::string temp = "";
    for (int i = 0; i < words.size(); i++) {
        //std::cout << 12 << " " << glutBitmapLength(font, reinterpret_cast<const unsigned char *>((words[i]).c_str())) << std::endl;
        if (glutBitmapLength(font, reinterpret_cast<const unsigned char *>((temp+words[i]).c_str())) > w) {
            stringsBuf.push_back(temp);
            temp = words[i];
        }
        else {
            temp = temp + words[i];
        }
    }
    stringsBuf.push_back(temp);
}

void text::reposCarrier() {
    int tempc = cursor;
    for (int i = 0; i < stringsBuf.size(); i++) {
        if (tempc > stringsBuf[i].length()) {
            tempc -= stringsBuf[i].length();
        }
        else {
            row = i;
            column = tempc;
            break;
        }
    }
}

void text::setfont(void* _font) {
    font = _font;
    stringHeight = 2*(double)glutBitmapHeight(font)/h;
}

void text::setbound(bool t) {
    bound = t;
}

void* const text::getfont() {
    return font;
}

bool const text::getbound() {
    return bound;
}

void text::resize(int _w, int _h, int _x, int _y) {
    w = _w;
    h = _h;
    x = _x;
    y = _y;

    generateStringsBuf();

    std::cout << "!!!" << stringsBuf.size() << std::endl;

    reposCarrier();
}

void text::draw() {
    if (!stringHeight) {
        stringHeight = 2*(double)glutBitmapHeight(font)/h;
    }
    glViewport(x, glutGet(GLUT_WINDOW_HEIGHT)-y-h, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor3f(0.0,0.0,0.0);
    for (int i = 0; i < stringsBuf.size(); i++) {
        renderString(-1.0f, 1.0f-stringHeight*(i+1), reinterpret_cast<const unsigned char *>(stringsBuf[i].c_str()));
        if (cursor != -1 &&  i == row) {
            if (counter > 0) {
                glBegin(GL_LINE_STRIP);
                glColor3f(0,0,0);
                glVertex2d(-1.0f + 2*(double)glutBitmapLength(font, reinterpret_cast<const unsigned char *>(stringsBuf[i].substr(0,column+1).c_str()))/w, 1.0-(i+1)*stringHeight);
                glVertex2d(-1.0f + 2*(double)glutBitmapLength(font, reinterpret_cast<const unsigned char *>(stringsBuf[i].substr(0,column+1).c_str()))/w, 1.0-(i)*stringHeight);
                glEnd();
            }
            counter--;
            if (counter < -timing) counter = timing;
        }
    }
    glEnable(GL_LIGHTING);

    if (bound) {
        glBegin(GL_LINE_STRIP);
        glVertex2d(-1,-1);
        glVertex2d(-1,1);
        glVertex2d(1,1);
        glVertex2d(1,-1);
        glVertex2d(-1,-1);
        glEnd();
    }
}

void text::renderString(float x, float y, const unsigned char * str) {
  glRasterPos2f(x, y);
  glutBitmapString(font, str);
}

void text::out() {
    for (auto &word : words) {
        std::cout << word << "_";
    }
    std::cout << std::endl;
}

int text::inside(int _x, int _y) {
std::cout << "y " << y << " _y " << _y << std::endl;
    if (_x < x || _y < y || _x > x+w || _y > y+h) {
        cursor = -1;
        return -1;
    }
    row = (_y-y)/glutBitmapHeight(font);
    if (row > stringsBuf.size()-1) {
        row = stringsBuf.size()-1;
    }
    column = -1;
    for (int i = 0; i < stringsBuf[row].length(); i++) {
        int _l = glutBitmapLength(font, reinterpret_cast<const unsigned char *>(stringsBuf[row].substr(0,i+1).c_str()));
        std::cout << _l << " - l; " << _x-x << std::endl;
        if (_l > _x-x) {
            column = i;

            break;
        }
    }
    if (column == -1) {
        column = stringsBuf[row].length();
    }
    int c = 0;
    for (int i = 0; i < row; i++) { // @todo: remove c, replace with cursor
        c+=stringsBuf[i].length();
    }
    c+=stringsBuf[row].substr(0,column).length();
    cursor=c;
    return row;
}

void text::keyboard(char _c) {
    if (cursor == -1) return;
    if (_c == 8) {
        if (s.length() > 0) {
            std::cout << s.substr(0,cursor) << "!!!!!!!!!! " << cursor << std::endl;
            s = s.substr(0,cursor) + (cursor < s.length()-1 ? s.substr(cursor+1,s.length()-cursor-1) : "");
            if (cursor > 0) cursor--;
            generateWords();
            generateStringsBuf();
            reposCarrier();
            counter = timing;
        }
    }
    else if ((_c >= 'A' && _c <= 'Z') || (_c >= 'a' && _c <= 'z') || (_c >= '(' && _c <= '9') || (_c == '^')) {
        if (s.length() > 0) {
            s = s.substr(0,cursor+1) + _c + (cursor < s.length()-1 ? s.substr(cursor+1,s.length()-cursor-1) : "");
        }
        else {
            s = _c;
        }
        cursor++;
        generateWords();
        generateStringsBuf();
        reposCarrier();
        counter = timing;
    }
}

void text::special(int key) {
    if (key == GLUT_KEY_LEFT) {
        if (cursor > 0) {
            cursor--;
            reposCarrier();
            counter = timing;
        }
    }
    else if (key == GLUT_KEY_RIGHT) {
        if (cursor < s.length()-1) {
            cursor++;
            reposCarrier();
            counter = timing;
        }
    }
}