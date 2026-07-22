echo "building for UNIX"

rm opengl

# gcc -I ./include opengl.c gameutil.c glad.c libglfw3_macos_universal.a -o "opengl" -lgdi -lopengl -luser
clang -g -I ./include opengl.c gameutil.c include/glad/glad.c libglfw3_macos_universal.a -o "opengl" -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lglfw3 -ldl

wait

# run inf successfullt generated an executable
if [ -f ./opengl ]; then
	echo "compiled successfully!" & 
	./opengl

echo "Finished"

# lldb comes with macos
lldb ./opengl
