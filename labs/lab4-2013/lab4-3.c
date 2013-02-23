// Lab 4, terrain generation

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

void make_vertice_normal(GLfloat *vertexArray, int x, int z, int width, bool up, Point3D *normal)
{
	Point3D v1, v2;
	v1.x = vertexArray[(x-1 + z * width)*3 + 0] 
		- vertexArray[(x + z * width)*3 + 0];
	v1.y = vertexArray[(x-1 + z * width)*3 + 1] 
		- vertexArray[(x + z * width)*3 + 1];
	v1.z = vertexArray[(x-1 + z * width)*3 + 2] 
		- vertexArray[(x + z * width)*3 + 2];
	if(up) {
		v2.x = vertexArray[(x-1 + (z+1) * width)*3 + 0] 
			- vertexArray[(x + z * width)*3 + 0];
		v2.y = vertexArray[(x-1 + (z+1) * width)*3 + 1] 
			- vertexArray[(x + z * width)*3 + 1];
		v2.z = vertexArray[(x-1 + (z+1) * width)*3 + 2] 
			- vertexArray[(x + z * width)*3 + 2];
		CrossProduct(&v1, &v2, normal);	
	} else {
		v2.x = vertexArray[(x + (z-1) * width)*3 + 0] 
			- vertexArray[(x + z * width)*3 + 0];
		v2.y = vertexArray[(x + (z-1) * width)*3 + 1] 
			- vertexArray[(x + z * width)*3 + 1];
		v2.z = vertexArray[(x + (z-1) * width)*3 + 2] 
			- vertexArray[(x + z * width)*3 + 2];
		CrossProduct(&v2, &v1, normal);	
	}
}

void put_vertex_normal(GLfloat *normalArray, int x, int z, int width, Point3D *normal)
{
	normalArray[(x + z * width)*3 + 0] += normal->x;
	normalArray[(x + z * width)*3 + 1] += normal->y;
	normalArray[(x + z * width)*3 + 2] += normal->z;
}


Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	for(int i = 0; i < 3*vertexCount;i++){
		normalArray[i] = 0;
	}
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);

	// Temporary vectors used for normal matrices
	Point3D normal;
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = 
                                tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
                                
			//vertexArray[(x + z * tex->width)*3 + 1] = 0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			if(x==0){
				NULL;
			} else if(x>1 && x<(tex->width-1) && z > 0 && z < (tex->height-1)){
				//printf("In general case: x=%d, z=%d\n", x, z);
				//Lower triangle
				make_vertice_normal(vertexArray, x, z, tex->width, false, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				ScalarMult(&normal, 1.0/6, &normal);
				put_vertex_normal(normalArray, x, z-1, tex->width, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);

				//Upper triangle
				make_vertice_normal(vertexArray, x, z, tex->width, true, &normal);
				ScalarMult(&normal, 1.0/6, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
				put_vertex_normal(normalArray, x-1, z+1, tex->width, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);
			} else if(x==1 && z==0){
				//printf("In bottom left corner: x=%d, z=%d\n", x, z);
				//Upper triangle
				make_vertice_normal(vertexArray, x, z, tex->width, true, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				put_vertex_normal(normalArray, 0, 0, tex->width, &normal);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, 0, 1, tex->width, &normal);
				put_vertex_normal(normalArray, 1, 1, tex->width, &normal);
			} else if(x==tex->width-1 && z == tex->height-1){
				//printf("In top right corner: x=%d, z=%d\n", x, z);
				//Lower triangle
				make_vertice_normal(vertexArray, x, z, tex->width, false, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, x, z-1, tex->width, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
			} else if(x==1 && z < tex->height-1){
				//printf("In left edge: x=%d, z=%d\n", x, z);
				// Left edge
				// Lower triangle
				make_vertice_normal(vertexArray, x, z, tex->width, false, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, x, z-1, tex->width, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
				ScalarMult(&normal, 1.0/2, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);

				// Upper Triangle
				make_vertice_normal(vertexArray, x, z, tex->width, true, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
				put_vertex_normal(normalArray, x-1, z+1, tex->width, &normal);
				ScalarMult(&normal, 1.0/2, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);
			} else if(z==0){
				//printf("In bottom edge: x=%d, z=%d\n", x, z);
				// Bottom edge
				// Upper Triangle
				make_vertice_normal(vertexArray, x, z, tex->width, true, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
				ScalarMult(&normal, 1.0/2, &normal);
				put_vertex_normal(normalArray, x-1, z+1, tex->width, &normal);
			} else if(x==tex->width-1 && z < tex->height-1){
				//printf("In right edge: x=%d, z=%d\n", x, z);
				// Right edge
				// Lower triangle
				make_vertice_normal(vertexArray, x, z, tex->width, false, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, x, z-1, tex->width, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);
				ScalarMult(&normal, 1.0/2, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);

				// Upper Triangle
				make_vertice_normal(vertexArray, x, z, tex->width, true, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);
				ScalarMult(&normal, 1.0/2, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
				put_vertex_normal(normalArray, x-1, z+1, tex->width, &normal);
			} else if(z==tex->height-1){
				//printf("In top edge: x=%d, z=%d\n", x, z);
				// Top edge
				// Lower triangle
				make_vertice_normal(vertexArray, x, z, tex->width, false, &normal);
				//printf("Normal: (%f,%f,%f)\n", normal.x, normal.y, normal.z);
				ScalarMult(&normal, 1.0/3, &normal);
				put_vertex_normal(normalArray, x-1, z, tex->width, &normal);
				put_vertex_normal(normalArray, x, z, tex->width, &normal);
				ScalarMult(&normal, 1.0/2, &normal);
				put_vertex_normal(normalArray, x, z-1, tex->width, &normal);
			}
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height;
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
