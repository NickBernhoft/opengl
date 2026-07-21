cls
del opengl.exe
gcc -g -I .\include opengl.c gameutil.c include\glad\glad.c libglfw3_mingw.a -o "opengl" -lgdi32 -lopengl32 -luser32
/wait
pause 

if exist ".\opengl.exe" gdb opengl.exe
:: pause