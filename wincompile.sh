i686-w64-mingw32-g++ -m32 -Wall -I/usr/i686-w64-mingw32/include/GL -o script.exe script.cpp function.cpp complex.cpp line.cpp curve.cpp ellipse.cpp text.cpp -D FREEGLUT_STATIC -L/usr/i686-w64-mingw32/lib  -lfreeglut_static -lopengl32 -lwinmm  -lgdi32 -Wl,--subsystem,windows -static-libgcc -static-libstdc++ -DFOR_WINDOWS

