// Lab 2-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.
//

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include"GL_utilities.h"
#include<math.h>
#include"loadobj.h"

// Globals
// Data would normally be read from files

Model *m;

GLfloat myTransMat[] = { 
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotMatX[] = { 
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotMatY[] = { 
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotMatZ[] = { 
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat t;

// Reference to shader program
GLuint program;

// vertex array object
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
//unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;

void init(void)
{
	dumpInfo();

        m = LoadModel("bunnyplus.obj");

	// GL inits
	glClearColor(0.0,0.3,0.3,0);
        glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-1.vert", "lab2-1.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
        glGenVertexArrays(1, &bunnyVertexArrayObjID);
        glGenBuffers(1, &bunnyVertexBufferObjID);
        glGenBuffers(1, &bunnyIndexBufferObjID);
        //glGenBuffers(1, &bunnyNormalBufferObjID);
        glGenBuffers(1, &bunnyTexCoordBufferObjID);

        glBindVertexArray(bunnyVertexArrayObjID);

        // VBO for vertex data
        glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

	printError("init vertex data");

        // VBO for normal data
        /*
        glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));

	printError("init normal data");
        */

        if (m->texCoordArray != NULL)
        {
                glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
                glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
                glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));

                printError("init texture data");
        }
        

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

        // End of upload of geometry
        
	printError("init arrays");
}

void display(void)
{
	printError("pre display");

        // Update rotation matrix
        t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

        myRotMatX[5] = cosf(0.003*t);
        //myRotMatX[6] = -sinf(0.001*t);
        //myRotMatX[9] = sinf(0.001*t);
        //myRotMatX[10] = cosf(0.001*t);

        myRotMatY[0] = cosf(0.001*t);
        myRotMatY[2] = sinf(0.001*t);
        myRotMatY[8] = -sinf(0.001*t);
        myRotMatY[10] = cosf(0.001*t);
        
        myRotMatZ[0] = cosf(0.002*t);
        //myRotMatZ[1] = -sinf(0.003*t);
        //myRotMatZ[4] = sinf(0.001*t);
        //myRotMatZ[5] = cosf(0.001*t);

        // Update translation matrix
        //myTransMat[3] = 0.5*cosf(0.002*t);
        //myTransMat[7] = 0.5*sinf(0.002*t);
        //myTransMat[11] = -0.7;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
        glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
        
        // Upload the Matrices
        //glUniformMatrix4fv(glGetUniformLocation(program, "myTransMat"), 1, GL_TRUE, myTransMat);
        glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatX"), 1, GL_TRUE, myRotMatX);
        glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatY"), 1, GL_TRUE, myRotMatY);
        glUniformMatrix4fv(glGetUniformLocation(program, "myRotMatZ"), 1, GL_TRUE, myRotMatZ);
        glUniformMatrix4fv(glGetUniformLocation(program, "myTransMat"), 1, GL_TRUE, myTransMat);
	
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
	glutCreateWindow ("GL3 Rave bunny example");
	glutDisplayFunc(display); 
	init();
        glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
