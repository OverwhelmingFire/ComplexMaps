x86_64-w64-mingw32-g++ -m64 -Wall -I/usr/x86_64-w64-mingw32/include/GL -o script64.exe script.cpp function.cpp complex.cpp -D FREEGLUT_STATIC -L/usr/x86_64-w64-mingw32/lib  -lfreeglut_static -lopengl32 -lwinmm  -lgdi32 -Wl,--subsystem,windows -static-libgcc -static-libstdc++ -DFOR_WINDOWS

