// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

#define PI 3.1415

GLfloat projectionMatrix[16];

// Camera stuffs!
Point3D obj_pos;
Point3D cam_pos;
Point3D vdiff;
Point3D intermediate;
Point3D up;

float move_speed = 0.4;

void check_keys(void);

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 8.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projectionMatrix);
	
	cam_pos.y = 5.0f;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	
	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);
	
// Load terrain data
	
	LoadTGATexture("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat total[16], modelView[16], camMatrix[16];
	
	printError("pre display");
	
	check_keys();

	glUseProgram(program);

	// Build matrix
	
	lookAt(&cam_pos, &obj_pos, up.x, up.y, up.z, camMatrix);
	IdentityMatrix(modelView);
	Mult(camMatrix, modelView, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}


void mouse(int x, int y)
{
  printf("%d, %d\n", x, y);
  float fi = ((float)x)/glutGet(GLUT_WINDOW_WIDTH)*2*PI;
  float theta = ((float)y)/glutGet(GLUT_WINDOW_HEIGHT)*PI;

  obj_pos.x = -10*sin(theta)*sin(fi) + cam_pos.x;
  obj_pos.y = 10*cos(theta) + cam_pos.y;
  obj_pos.z = 10*sin(theta)*cos(fi) + cam_pos.z;
}

void check_keys(void){
        if(keyIsDown('w')){
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                Normalize(&vdiff);
                VectorAdd(&vdiff, &cam_pos, &cam_pos);
                VectorAdd(&vdiff, &obj_pos, &obj_pos);
        } else if (keyIsDown('s')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                Normalize(&vdiff);
                VectorSub(&cam_pos, &vdiff, &cam_pos);
                VectorSub(&obj_pos, &vdiff, &obj_pos);
        } else if (keyIsDown('a')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                CrossProduct(&up, &vdiff, &vdiff);
                Normalize(&vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                VectorAdd(&vdiff, &cam_pos, &cam_pos);
                VectorAdd(&vdiff, &obj_pos, &obj_pos);
        } else if (keyIsDown('d')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                CrossProduct(&up, &vdiff, &vdiff);
                Normalize(&vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                VectorSub(&cam_pos, &vdiff, &cam_pos);
                VectorSub(&obj_pos, &vdiff, &obj_pos);                                
        } else if (keyIsDown('q')) {
                exit(0);
        }
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
