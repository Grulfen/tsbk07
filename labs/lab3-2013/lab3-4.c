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

#define PI 3.14

GLfloat projectionMatrix[] = {    
        2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
        0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
        0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
        0.0f, 0.0f, -1.0f, 0.0f };

// Globals
// Data would normally be read from files

Model *mill, *blade, *roof, *balcony, *ground, *bunny, *skybox;

GLfloat t;
GLfloat dr = 0;
GLfloat r = 0;

GLfloat rotx[16], roty[16], rotz[16], tilt[16],  trans[16], trans1[16], total[16], cam_Matrix[16], cam_Matrix_skybox[16];

GLuint myTex;
GLuint enable_skybox;

// Declare texture reference
//GLuint myBilTex;

// Reference to shader program
GLuint program;

// Camera stuffs!
Point3D obj_pos;
Point3D cam_pos;
Point3D vdiff;
Point3D intermediate;
Point3D up;

float move_speed = 0.4;

// Rotation stuffs!
float omega_x = -0.003;
float omega_y = 0.000;
float omega_z = 0.001;

float move_x = 0;
float move_y = 0;
float move_z = -20;

float move_before_x = 4.6;
float move_before_y = 0.5;
float move_before_z = 0.0;

void draw_object(Model *model, char *texture, GLfloat *total)
{
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        if(texture != NULL){
                LoadTGATextureSimple(texture, &myTex);
                glBindTexture(GL_TEXTURE_2D, myTex);
        }
        DrawModel(model, program, "inPosition", "inNormal", "inTexCoord");
}

void draw_blade(float rot, float pos_x, float pos_y, float pos_z)
{
        T(move_before_x,move_before_y,move_before_z, trans);
        Ry(omega_y*t, roty);
        Rx(rot + omega_x*r, rotx);
        Mult(rotx, tilt, total);
        Mult(roty, total, total);
        Mult(total, trans, total);
        T(pos_x,pos_y,pos_z, trans);
        Mult(trans, total, total);
        draw_object(blade, NULL, total);
}

void draw_windmill(float pos_x, float pos_y, float pos_z)
{
        LoadTGATextureSimple("grass.tga", &myTex);
        glBindTexture(GL_TEXTURE_2D, myTex);

        T(pos_x,pos_y,pos_z, trans);
        Ry(omega_y*t, roty);
        Mult(trans, roty, total);
        draw_object(mill, NULL, total);

        T(pos_x,pos_y,pos_z, trans);
        Ry(omega_y*t, roty);
        Mult(trans, roty, total);
        draw_object(balcony, NULL, total);

        T(pos_x,pos_y,pos_z, trans);
        Ry(omega_y*t, roty);
        Mult(trans, roty, total);
        draw_object(roof, NULL, total);

        Ry(0.1, tilt);

        draw_blade(PI, pos_x, pos_y + 9.2, pos_z);
        draw_blade(PI/2, pos_x, pos_y + 9.2, pos_z);
        draw_blade(3*PI/2, pos_x, pos_y + 9.2, pos_z);
        draw_blade(0, pos_x, pos_y + 9.2, pos_z);
}

void init(void)
{
	dumpInfo();

        mill = LoadModelPlus( "windmill/windmill-walls.obj");
        blade = LoadModelPlus( "windmill/blade.obj");
        balcony = LoadModelPlus( "windmill/windmill-balcony.obj");
        roof = LoadModelPlus( "windmill/windmill-roof.obj");
        ground = LoadModelPlus( "ground.obj");
        bunny = LoadModelPlus("bunnyplus.obj");
        skybox = LoadModelPlus("skybox.obj");

	// GL inits
	glClearColor(0.0,0.3,0.3,0);
        //glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-4.vert", "lab3-4.frag");
	printError("init shader");
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

	printError("init arrays");

        // Init up vector
        up.x = 0;
        up.y = 1;
        up.z = 0;

        cam_pos.x = 0;
        cam_pos.y = 5;
        cam_pos.z = 0;

        obj_pos.x = 0;
        obj_pos.y = 5;
        obj_pos.z = -30;

        // Init light
        
        Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                {0.0f, 1.0f, 0.0f}, // Green light
                {0.0f, 0.0f, 1.0f}, // Blue light
                {1.0f, 1.0f, 1.0f} }; // White light

        GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
        GLint isDirectional[] = {0,0,1,1};

        Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                {0.0f, 5.0f, 10.0f}, // Green light, positional
                {-1.0f, 0.0f, 0.0f}, // Blue light along X
                {0.0f, 0.0f, -1.0f} }; // White light along Z

        glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
        glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
        glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
        glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

        initKeymapManager();
}

void check_keys(void){
        if(keyIsDown('w')){
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                vdiff.y = 0;
                ScalarMult(&vdiff, move_speed, &vdiff);
                Normalize(&vdiff);
                VectorAdd(&vdiff, &cam_pos, &cam_pos);
                VectorAdd(&vdiff, &obj_pos, &obj_pos);
        } else if (keyIsDown('s')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                vdiff.y = 0;
                ScalarMult(&vdiff, move_speed, &vdiff);
                Normalize(&vdiff);
                VectorSub(&cam_pos, &vdiff, &cam_pos);
                VectorSub(&obj_pos, &vdiff, &obj_pos);
        } else if (keyIsDown('a')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                vdiff.y = 0;
                CrossProduct(&up, &vdiff, &vdiff);
                Normalize(&vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                VectorAdd(&vdiff, &cam_pos, &cam_pos);
                VectorAdd(&vdiff, &obj_pos, &obj_pos);
        } else if (keyIsDown('d')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                vdiff.y = 0;
                CrossProduct(&up, &vdiff, &vdiff);
                Normalize(&vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                VectorSub(&cam_pos, &vdiff, &cam_pos);
                VectorSub(&obj_pos, &vdiff, &obj_pos);
        } else if (keyIsDown('u')) {
                cam_pos.y += 0.1;
                obj_pos.y += 0.1;
        } else if (keyIsDown('j')) {
                cam_pos.y -= 0.1;
                obj_pos.y -= 0.1;
        } else if (keyIsDown('r')) {
                if(dr < 80){
                        dr += 0.5; 
                }
        } else if (keyIsDown('t')) {
                if(dr > -80){
                        dr -= 0.5; 
                }
        } else if (keyIsDown('q')) {
                exit(0);
        }
}

void display(void)
{
	printError("pre display");

        t += 2;

        check_keys();

        r += dr;

        lookAt(&cam_pos, &obj_pos, up.x, up.y, up.z, cam_Matrix);

        CopyMatrix(cam_Matrix, cam_Matrix_skybox);
        cam_Matrix_skybox[3] = 0;
        cam_Matrix_skybox[7] = 0;
        cam_Matrix_skybox[11] = 0;
        cam_Matrix_skybox[15] = 1;

        // Fixes for skybox and ground
        glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1i(glGetUniformLocation(program, "skybox"), 1);

        glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_Matrix_skybox);
        T(0,-0.5,0,total);
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        LoadTGATextureSimple("SkyBox512.tga", &myTex);
        glBindTexture(GL_TEXTURE_2D, myTex);
        DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");
        

        glUniform1i(glGetUniformLocation(program, "skybox"), 0);

        // Upload time
        glUniform1f(glGetUniformLocation(program, "t"), t); // Time

	// clear the screen
        glEnable(GL_DEPTH_TEST);

        // Upload new camera matrix
        glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_Matrix);

        // Upload the Matrices
        draw_windmill(move_x, move_y, move_z);
        
        IdentityMatrix(total);
        draw_object(ground, "dirt.tga", total);

        for(int x=0;x<1; x++){
                for(int y=0;y<1; y++){
                        T(2*x-2,0.5, 2*y-2, total);
                        draw_object(bunny, "maskros512.tga", total);
                }
        }

        glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int value)
{
        glutPostRedisplay();
        glutTimerFunc(20, &OnTimer, value);
}

void mouse(int x, int y)
{
        float fi = ((float)x)/glutGet(GLUT_WINDOW_WIDTH)*2*PI;
        float theta = ((float)y)/glutGet(GLUT_WINDOW_HEIGHT)*PI;

        obj_pos.x = -10*sin(theta)*sin(fi) + cam_pos.x;
        obj_pos.y = 10*cos(theta) + cam_pos.y;
        obj_pos.z = 10*sin(theta)*cos(fi) + cam_pos.z;
}

int main(int argc, const char *argv[])
{
	glutInit(&argc, (char**)argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(640, 480);
	glutCreateWindow ("GL3 external light sources and windmill example");
	glutDisplayFunc(display); 
        glutPassiveMotionFunc(mouse);
	init();
        glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
