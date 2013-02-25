
all : lab4-1 lab4-2 lab4-3 lab4-4

lab4-1: lab4-1.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o lab4-1 -DGL_GLEXT_PROTOTYPES lab4-1.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c  -lGL -lglut -lm

lab4-2: lab4-2.c GL_utilities.c
	gcc -Wall -std=c99 -o lab4-2 -DGL_GLEXT_PROTOTYPES lab4-2.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c  -lGL -lglut -lm

lab4-3: lab4-3.c GL_utilities.c
	gcc -Wall -std=c99 -o lab4-3 -DGL_GLEXT_PROTOTYPES lab4-3.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c  -lGL -lglut -lm

lab4-4: lab4-4.c GL_utilities.c
	gcc -Wall -std=c99 -o lab4-4 -DGL_GLEXT_PROTOTYPES lab4-4.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c  -lGL -lglut -lm

clean :
	rm lab4-1
	rm lab4-2	rm lab4-3	rm lab4-4