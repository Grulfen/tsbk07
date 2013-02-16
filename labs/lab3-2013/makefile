all : lab3-1 lab3-2 lab3-3 lab3-4

lab3-1: lab3-1.c GL_utilities.c 
	gcc -Wall -std=c99 -o lab3-1 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab3-1.c GL_utilities.c loadobj.c VectorUtils2.c LoadTGA.c

lab3-2: lab3-2.c GL_utilities.c
	gcc -Wall -std=c99 -o lab3-2 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab3-2.c GL_utilities.c loadobj.c VectorUtils2.c LoadTGA.c

lab3-3: lab3-3.c GL_utilities.c
	gcc -Wall -std=c99 -o lab3-3 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab3-3.c GL_utilities.c loadobj.c VectorUtils2.c LoadTGA.c

lab3-4: lab3-4.c GL_utilities.c
	gcc -Wall -std=c99 -o lab3-4 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab3-4.c GL_utilities.c loadobj.c VectorUtils2.c LoadTGA.c

clean :
	rm lab3-1
	rm lab3-2
	rm lab3-3	rm lab3-4