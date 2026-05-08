cls
del opengl.exe
gcc -I .\include opengl.c gameutil.c glad.c libglfw3.a -o "opengl" -lgdi32 -lopengl32 -luser32
/wait
pause 

if exist ".\opengl.exe" ".\opengl.exe"
:: pause