all : lab2-1 lab2-2 lab2-3 lab2-4 lab2-5 lab2-7 lab2-8

lab2-1: lab2-1.c GL_utilities.c 
	gcc -Wall -std=c99 -o lab2-1 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-1.c GL_utilities.c loadobj.c

lab2-2: lab2-2.c GL_utilities.c
	gcc -Wall -std=c99 -o lab2-2 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-2.c GL_utilities.c loadobj.c LoadTGA.c

lab2-3: lab2-3.c GL_utilities.c
	gcc -Wall -std=c99 -o lab2-3 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-3.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c

lab2-4: lab2-4.c GL_utilities.c
	gcc -Wall -std=c99 -o lab2-4 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-4.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c

lab2-5: lab2-5.c GL_utilities.c
	gcc -Wall -std=c99 -o lab2-5 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-5.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c

lab2-7: lab2-7.c GL_utilities.c
	gcc -Wall -std=c99 -o lab2-7 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-7.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c

lab2-8: lab2-8.c GL_utilities.c
	gcc -Wall -std=c99 -o lab2-8 -lm -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-8.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c

clean :
	rm lab2-1
	rm lab2-2
	rm lab2-3
	rm lab2-4	
	rm lab2-5
	rm lab2-7
	rm lab2-8