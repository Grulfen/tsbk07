// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include "GL_utilities.h"
#include<math.h>

// Globals
// Data would normally be read from files
GLfloat vertices[] = {
        //bottom
        0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        //---------------
        0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,0.5f,-0.5f,
        //###############

        //front
        0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        //---------------
        0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,
        0.5f,-0.5f,0.5f,
        //###############

        //back
        0.5f,0.5f,-0.5f,
        -0.5f,0.5f,-0.5f,
        -0.5f,0.5f,0.5f,
        //--------------
        0.5f,0.5f,-0.5f,
        -0.5f,0.5f,0.5f,
        0.5f,0.5f,0.5f,
        //##############

        //left
        -0.5f,0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,0.5f,-0.5f,
        //----------------
        -0.5f,0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,
        //################

        //right
        0.5f,0.5f,0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,0.5f,-0.5f,
        //---------------
        0.5f,0.5f,0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,0.5f,
        //###############
        
        //top
        0.5f,0.5f,0.5f,
        -0.5f,-0.5f,0.5f,
        0.5f,-0.5f,0.5f,
        //--------------
        0.5f,0.5f,0.5f,
        -0.5f,-0.5f,0.5f,
        -0.5f,0.5f,0.5f
};
GLfloat colors[] = {
        //bottom
        0.5f,0.0f,0.2f,
        0.5f,0.0f,0.4f,
        0.5f,0.0f,0.6f,
        //-------------
        0.5f,0.0f,0.2f,
        0.5f,0.0f,0.4f,
        0.5f,0.2f,0.0f,
        //#############

        //front
        0.5f,0.2f,0.2f,
        0.5f,0.2f,0.4f,
        0.5f,0.2f,0.6f,
        //-------------
        0.5f,0.2f,0.2f,
        0.5f,0.2f,0.4f,
        0.5f,0.4f,0.0f,
        //#############

        //back
        0.5f,0.4f,0.2f,
        0.5f,0.4f,0.4f,
        0.5f,0.4f,0.6f,
        //-------------
        0.5f,0.4f,0.2f,
        0.5f,0.4f,0.4f,
        0.5f,0.6f,0.0f,
        //#############

        //left
        0.5f,0.6f,0.2f,
        0.5f,0.6f,0.4f,
        0.5f,0.6f,0.6f,
        //-------------
        0.5f,0.6f,0.2f,
        0.5f,0.6f,0.4f,
        0.5f,0.8f,0.0f,
        //#############

        //right
        0.5f,0.8f,0.2f,
        0.5f,0.8f,0.4f,
        0.5f,0.8f,0.6f,
        //-------------
        0.5f,0.8f,0.2f,
        0.5f,0.8f,0.4f,
        0.5f,1.0f,0.0f,
        //#############
        
        //top
        0.5f,1.0f,0.2f,
        0.5f,1.0f,0.4f,
        0.5f,1.0f,0.6f,
        //-------------
        0.5f,1.0f,0.2f,
        0.5f,1.0f,0.4f,
        1.0f,0.0f,0.0f
        //#############
};

GLfloat myTransMat[] = { 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotMatX[] = { 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotMatY[] = { 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotMatZ[] = { 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat t;

// Reference to shader program
GLuint program;

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.0,0.3,0.3,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(2, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &colorBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 108*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
        
	// VBO for color data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 108*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
	
	// End of upload of geometry
        
	printError("init arrays");
}

void display(void)
{
	printError("pre display");

        // Update rotation matrix
        t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

        myRotMatZ[0] = cosf(0.001*t);
        myRotMatZ[1] = -sinf(0.001*t);
        myRotMatZ[4] = sinf(0.001*t);
        myRotMatZ[5] = cosf(0.001*t);

        myRotMatX[5] = cosf(0.002*t);
        myRotMatX[6] = -sinf(0.002*t);
        myRotMatX[9] = sinf(0.002*t);
        myRotMatX[10] = cosf(0.002*t);

        myRotMatY[0] = cosf(0.001*t);
        myRotMatY[2] = sinf(0.001*t);
        myRotMatY[8] = -sinf(0.001*t);
        myRotMatY[10] = cosf(0.001*t);

        // Update translation matrix
        myTransMat[3] = 0.5*cosf(0.002*t);
        myTransMat[7] = 0.5*sinf(0.002*t);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	//glDrawArrays(GL_TRIANGLES_ADJACENCY, 0, 36);	// draw object
	glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object
        
        // Upload the Matrices
        glUniformMatrix4fv(glGetUniformLocation(program, "myTransMat"), 1, GL_TRUE, myTransMat);
        glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatX"), 1, GL_TRUE, myRotMatX);
        glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatY"), 1, GL_TRUE, myRotMatY);
        glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatZ"), 1, GL_TRUE, myRotMatZ);
	
	printError("display");
	
	//glFlush();
	glutSwapBuffers();
}

void OnTimer(int value)
{
        glutPostRedisplay();
        glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, const char *argv[])
{
	glutInit(&argc, (char**)argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("GL3 cube example");
	glutDisplayFunc(display); 
	init();
        glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
