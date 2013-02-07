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

Model *mill;
Model *blade;

GLfloat t;

GLfloat rot[16], trans[16], total[16], cam_Matrix[16];
//GLfloat roty2[16], rotx2[16], trans2[16], total2[16], cam_Matrix2[16];
//GLfloat rot3[16], trans3[16], total3[16], cam_Matrix3[16];

// Declare texture reference
//GLuint myBilTex;

// Reference to shader program
GLuint program;

// Camera stuffs!
Point3D obj_pos;
Point3D cam_pos;
GLfloat up[3];

void init(void)
{
	dumpInfo();

        mill = LoadModelPlus( "windmill/windmill-walls.obj");
        blade = LoadModelPlus( "windmill/blade.obj");

	// GL inits
	glClearColor(0.0,0.3,0.3,0);
        glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");

        // Load texture
        //LoadTGATextureSimple("bilskissred.tga", &myBilTex);

	printError("init arrays");
}

void display(void)
{
	printError("pre display");

        // Update rotation matrix
        t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

        T(0,0,-4, total);
        //Ry(3.14/2, rot);
        //Mult(trans, rot, total);

        // Camera stuff
        obj_pos.x = 0;
        obj_pos.y = 0;
        obj_pos.z = -4;
        cam_pos.x = 6*cos(0.0005*t);
        cam_pos.y = 2;
        cam_pos.z = -4 + 6*sin(0.0005*t);
        up[0] = 0;
        up[1] = 1;
        up[2] = 0;

        lookAt(&cam_pos, &obj_pos, up[0], up[1], up[2], cam_Matrix);
        
        // Upload time
        glUniform1f(glGetUniformLocation(program, "t"), t); // Time


	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        
        // Active texture object
        //glBindTexture(GL_TEXTURE_2D, myBilTex);
        DrawModel(mill, program, "inPosition", "inNormal", "inTexCoord");
        
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
	glutCreateWindow ("GL3 texture and bunnies example");
	glutDisplayFunc(display); 
	init();
        glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
