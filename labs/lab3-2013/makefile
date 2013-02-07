all : lab3-1

lab3-1: lab3-1.c GL_utilities.c 
	gcc -Wall -std=c99 -o lab3-1 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab3-1.c GL_utilities.c loadobj.c VectorUtils2.c LoadTGA.c

clean :
	rm lab3-1
