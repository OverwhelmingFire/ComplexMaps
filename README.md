# ComplexMaps
A small program to visualize complex functions

This program is created in sake of better understanding how various shapes on a complex plane are transformed when complex functions are applied to them.

## 1. What can I do in this program?
### Drawing
You can draw different kinds of shapes (curves, straight lines and ellipses by now). The kind of a shape is defined by the current mode. You can switch between the modes pressing 1,2 and 3 on the keyboard.  
* 1 stands for curves
* 2 stands for straight lines
* 3 stands for ellipses  
To draw a shape, you should press the left mouse button anywhere on the screen, move the mouse around and release it. In the ellipse mode you can also regulate the width of an ellipse scrolling the mouse wheel.  

### Setting a custom function
You can enter a custom function in a text box on the right. All axises and your paintings will be re-mapped. However, the function must meet some syntax requirements:
1. There must not be any excessive brackets
2. There must not be 2 and more operators in a row
3. Currently the following operators are supported: ```+ - * / ^```
4. Currently the following functions are supported:
    1. ```sin(), or s(); cos(), or c()```
    2. ```exp(), or e(); ln(), or log(), or l()```

## 2. How can I launch this program?

### Download pre-compiled executable files...
First of all, there are pre-compiled executable files, which you can download and launch if you dare. 

#### - Linux:
Download **script** and run it from the command prompt (assuming that your current directory is the one containing the file):  
```$ ./script```

#### - Windows:
Download **script.exe** and run it as an ordinary executable file.

### ...or compile by your own.
Moreover, you can compile executable by yourself.

#### - On Linux for Linux:

#### - On Linux for Windows:

Firstly, Mingw-w64 will come in handy while compiling on Linux for Windows:  
```$ sudo apt-get install mingw-w64```  
Then, you should download freeglut for windows. Otherwise you will just run into a bunch of errors. You can find it [here](https://www.transmissionzero.co.uk/software/freeglut-devel/). 
Just scroll down for the **freeglut 3.0.0 MSVC Package**. There are two important folders: **include** and **lib**.  
```├-- include
├-- lib
│   ├-- x64
│   ├-- ...
└ ...
```
If you want to compile for x32 and x64, copy everything inside of **include** and everything **(but x64)** inside of **lib** to **/usr/i686-w64-mingw32/**. Switch to the directory with downloaded repo and run the following command:  
```$ i686-w64-mingw32-g++ -m32 -Wall -I/usr/i686-w64-mingw32/include/GL -o script.exe script.cpp function.cpp complex.cpp -D FREEGLUT_STATIC -L/usr/i686-w64-mingw32/lib  -lfreeglut_static -lopengl32 -lwinmm  -lgdi32 -Wl,--subsystem,windows -static-libgcc -static-libstdc++```  
If you want to compile for x64, copy everything inside of **include** and everything inside of **lib/x64** to **/usr/x86_64-w64-mingw32/**. Switch to the directory with downloaded repo and run the following command:  
```$ x86_64-w64-mingw32-g++ -m64 -Wall -I/usr/x86_64-w64-mingw32/include/GL -o script64.exe script.cpp function.cpp complex.cpp -D FREEGLUT_STATIC -L/usr/x86_64-w64-mingw32/lib  -lfreeglut_static -lopengl32 -lwinmm  -lgdi32 -Wl,--subsystem,windows -static-libgcc -static-libstdc++```  
Executable file will be generated. Launching of the executables under different systems was described in the previous paragraph. 
