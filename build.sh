echo "building for UNIX"

rm opengl

gcc -I ./include opengl.c gameutil.c glad.c libglfw3.a -o "opengl" -lgdi -lopengl -luser
wait

# run inf successfullt generated an executable
if [ -f ./opengl ]; then
	echo "compiled successfully!" & 
	./opengl

echo "Finished"
