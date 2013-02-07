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

Model *mill, *blade, *roof, *balcony;

GLfloat t;
GLfloat r = 0;

GLfloat rotx[16], roty[16], rotz[16], tilt[16],  trans[16], trans1[16], total[16], cam_Matrix[16];

// Declare texture reference
//GLuint myBilTex;

// Reference to shader program
GLuint program;

// Camera stuffs!
Point3D obj_pos;
Point3D cam_pos;
GLfloat up[3];

// Rotation stuffs!
float omega_x = -0.003;
float omega_y = 0.0005;
float omega_z = 0.001;

float move_x = 0.0;
float move_y = 9.2;
float move_z = -20;

float move_before_x = 4.6;
float move_before_y = 0.5;
float move_before_z = 0.0;

void init(void)
{
	dumpInfo();

        mill = LoadModelPlus( "windmill/windmill-walls.obj");
        blade = LoadModelPlus( "windmill/blade.obj");
        balcony = LoadModelPlus( "windmill/windmill-balcony.obj");
        roof = LoadModelPlus( "windmill/windmill-roof.obj");

	// GL inits
	glClearColor(0.0,0.3,0.3,0);
        glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-2.vert", "lab3-2.frag");
	printError("init shader");

        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

        // Load texture
        //LoadTGATextureSimple("bilskissred.tga", &myBilTex);

	printError("init arrays");


        // Init camera
        up[0] = 0;
        up[1] = 1;
        up[2] = 0;

        obj_pos.x = 0;
        obj_pos.y = 5;
        obj_pos.z = -20;
        cam_pos.x = 0;
        cam_pos.y = 10;
        cam_pos.z = 0;

        initKeymapManager();
}

void check_keys(void){
        if(keyIsDown('w')){
                cam_pos.z -= 0.1;
        } else if (keyIsDown('a')) {
                cam_pos.x -= 0.1;
        } else if (keyIsDown('s')) {
                cam_pos.z += 0.1;
        } else if (keyIsDown('d')) {
                cam_pos.x += 0.1;
        } else if (keyIsDown('u')) {
                cam_pos.y += 0.1;
        } else if (keyIsDown('j')) {
                cam_pos.y -= 0.1;
        } else if (keyIsDown('r')) {
                r += 10; 
        } else if (keyIsDown('t')) {
                r -= 10; 
        } else if (keyIsDown('q')) {
                exit(0);
        }
}

void display(void)
{
	printError("pre display");

        //t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
        t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

        check_keys();

        r += 5;

        lookAt(&cam_pos, &obj_pos, up[0], up[1], up[2], cam_Matrix);

        glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_Matrix);
        
        // Upload time
        glUniform1f(glGetUniformLocation(program, "t"), t); // Time


	// clear the screen
        glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Active texture object
        // glBindTexture(GL_TEXTURE_2D, myBilTex);
        
        // Upload the Matrices

        T(0.0,0.0,-20.0, trans);
        Ry(omega_y*t, roty);
        Mult(trans, roty, total);

        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(mill, program, "inPosition", "inNormal", "inTexCoord");

        T(0.0,0.0,-20.0, trans);
        Ry(omega_y*t, roty);
        Mult(trans, roty, total);
        
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(balcony, program, "inPosition", "inNormal", "inTexCoord");

        T(0.0,0.0,-20, trans);
        Ry(omega_y*t, roty);
        Mult(trans, roty, total);

        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(roof, program, "inPosition", "inNormal", "inTexCoord");

        Ry(0, tilt);

        T(move_before_x,move_before_y,move_before_z, trans);
        Ry(omega_y*t, roty);
        Rx(3.14 + omega_x*r, rotx);
        Mult(rotx, tilt, total);
        Mult(roty, total, total);
        Mult(total, trans, total);
        T(move_x,move_y,move_z, trans);
        Mult(trans, total, total);

        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(blade, program, "inPosition", "inNormal", "inTexCoord");

        T(move_before_x,move_before_y,move_before_z, trans);
        Ry(omega_y*t, roty);
        Rx(3.14/2 + omega_x*r, rotx);
        Mult(rotx, tilt, total);
        Mult(roty, total, total);
        Mult(total, trans, total);
        T(move_x,move_y,move_z, trans);
        Mult(trans, total, total);


        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(blade, program, "inPosition", "inNormal", "inTexCoord");

        T(move_before_x,move_before_y,move_before_z, trans);
        Ry(omega_y*t, roty);
        Rx(-3.14/2 + omega_x*r, rotx);
        Mult(rotx, tilt, total);
        Mult(roty, total, total);
        Mult(total, trans, total);
        T(move_x,move_y,move_z, trans);
        Mult(trans, total, total);

        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(blade, program, "inPosition", "inNormal", "inTexCoord");

        T(move_before_x,move_before_y,move_before_z, trans);
        Ry(omega_y*t, roty);
        Rx(0 + omega_x*r, rotx);
        Mult(rotx, tilt, total);
        Mult(roty, total, total);
        Mult(total, trans, total);
        T(move_x,move_y,move_z, trans);
        Mult(trans, total, total);



        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(blade, program, "inPosition", "inNormal", "inTexCoord");
        

        // Upload texture unit
        //glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

	printError("display");
	
	//glFlush();
	glutSwapBuffers();
}

void OnTimer(int value)
{
        glutPostRedisplay();
        glutTimerFunc(20, &OnTimer, value);
}


void mouse(int x, int y)
{
        glUniform2f(glGetUniformLocation(program, "coords"), (GLfloat)x, (GLfloat)y);
}


int main(int argc, const char *argv[])
{
	glutInit(&argc, (char**)argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("GL3 texture and bunnies example");
	glutDisplayFunc(display); 
        glutPassiveMotionFunc(mouse);
	init();
        glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
