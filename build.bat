cls
del opengl.exe
gcc -I .\include opengl.c gameutil.c include\glad\glad.c libglfw3_mingw.a -o "opengl" -lgdi32 -lopengl32 -luser32
/wait
pause 

if exist ".\opengl.exe" ".\opengl.exe"
:: pause