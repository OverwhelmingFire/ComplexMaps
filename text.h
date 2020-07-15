#ifndef TEXTS
#define TEXTS

#include "primitive.h"

// @todo: overload cout and cin for this class

class text : public primitive {
    private:
        static const int timing = 5;
        bool bound;
        double stringHeight;
        int cursor = -1; // @todo: change cursor=>carrier
        int counter = timing;
        int row;
        int column;
        int l = 0;
        int w;
        int h;
        int x;
        int y;
        void* font;
        std::string s;
        std::vector<std::string> words;
        std::vector<std::string> stringsBuf;
        void renderString(float x, float y, const unsigned char * str);
        void generateWords();
        void generateStringsBuf();
        void reposCarrier();
    public:
        text();
        text(int const _w, int const _h);
        text(int const _w, int const _h, int _x, int _y, std::string _s);
        void setfont(void* _font);
        void* const getfont();
        void setbound(bool t);
        bool const getbound();
        virtual void resize(int _w, int _h, int _x, int _y);
        virtual void draw();
        virtual void out();
        void keyboard(char _c);
        void special(int key);
        int inside(int x, int y);
};

#endif