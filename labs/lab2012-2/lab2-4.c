#include"GL_utilities.h"
#include<math.h>
#include"loadobj.h"
#include"LoadTGA.h"
#include"VectorUtils2.h"


#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat projectionMatrix[] = {    
        2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
        0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
        0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
        0.0f, 0.0f, -1.0f, 0.0f };

// Globals
// Data would normally be read from files

Model *m;

GLfloat t;

GLfloat rot[16], trans[16], total[16], cam_Matrix[16];

// Declare texture reference
GLuint myTex;

// Reference to shader program
GLuint program;

// Camera stuffs!
Point3D obj_pos;
Point3D cam_pos;
GLfloat up[3];

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
	program = loadShaders("lab2-4.vert", "lab2-4.frag");
	printError("init shader");

        // Load texture
        LoadTGATextureSimple("grass.tga", &myTex);

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

        // Active texture object
        glBindTexture(GL_TEXTURE_2D, myTex);
        

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

        T(0,0,-3, trans);
        Ry(0.001*t, rot);
        Mult(trans, rot, total);

        // Camera stuff
        obj_pos.x = 0;
        obj_pos.y = 0;
        obj_pos.z = -3;
        cam_pos.x = 1 + sin(0.001*t);
        cam_pos.y = 1 + sin(0.001*t);
        cam_pos.z = sin(0.001*t);
        up[0] = 1;
        up[1] = 0;
        up[2] = 0;

        lookAt(&cam_pos, &obj_pos, up[0], up[1], up[2], cam_Matrix);


	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
        glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
        
        // Upload the Matrices
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_Matrix);
        
        // Upload texture unit
        glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	
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
	glutCreateWindow ("GL3 texture and bunny example");
	glutDisplayFunc(display); 
	init();
        glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
