// Lab 4, terrain generation

#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

#define PI 3.1415
#define cam_height 3

GLfloat projectionMatrix[16];

// Camera stuffs!
Point3D obj_pos;
Point3D cam_pos;
Point3D vdiff;
Point3D intermediate;
Point3D up;

GLfloat *vertexArray;

float move_speed = 0.4;

void check_keys(void);

float calculate_height(float x, float z, int width, GLfloat *vertexArray)
{
        int quad = (floor(x) + floor(z)*width)*3;
        // Chooses upper or lower triangle, 1 = upper, 0 = lower
        int triangle = (((x - floor(x))+(z - floor(z))) > 1)? 1 : 0; 
        Point3D corner1, corner2, corner3;
        if(triangle){
                // Upper triangle 
                int u = 1;
                int w = 1;
                corner1.x = vertexArray[quad + (u + w*width)*3 + 0];
                corner1.y = vertexArray[quad + (u + w*width)*3 + 1];
                corner1.z = vertexArray[quad + (u + w*width)*3 + 2];
                u = 0;
                corner2.x = vertexArray[quad + (u + w*width)*3 + 0];
                corner2.y = vertexArray[quad + (u + w*width)*3 + 1];
                corner2.z = vertexArray[quad + (u + w*width)*3 + 2];
                u = 1;
                w = 0;
                corner3.x = vertexArray[quad + (u + w*width)*3 + 0];
                corner3.y = vertexArray[quad + (u + w*width)*3 + 1];
                corner3.z = vertexArray[quad + (u + w*width)*3 + 2];
        } else {
                // Lower triangle 
                int u = 0;
                int w = 0;
                corner1.x = vertexArray[quad + (u + w*width)*3 + 0];
                corner1.y = vertexArray[quad + (u + w*width)*3 + 1];
                corner1.z = vertexArray[quad + (u + w*width)*3 + 2];
                u = 1;
                corner2.x = vertexArray[quad + (u + w*width)*3 + 0];
                corner2.y = vertexArray[quad + (u + w*width)*3 + 1];
                corner2.z = vertexArray[quad + (u + w*width)*3 + 2];
                u = 0;
                w = 1;
                corner3.x = vertexArray[quad + (u + w*width)*3 + 0];
                corner3.y = vertexArray[quad + (u + w*width)*3 + 1];
                corner3.z = vertexArray[quad + (u + w*width)*3 + 2];
        }
        Point3D v1, v2, normal;
        VectorSub(&corner2, &corner1, &v1);
        VectorSub(&corner3, &corner1, &v2);
        CrossProduct(&v1,&v2,&normal);
        // Plane equation = A*x + B*y + C*z + D = 0
        float A,B,C,D;
        A = normal.x;
        B = normal.y;
        C = normal.z;
        D = -A*corner1.x - B*corner1.y - C*corner1.z;
        // y = - (D + A*x + C*z) / B
        return -(D + A*x + C*z) / B;
}

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
	
	//GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	for(int i = 0; i < 3*vertexCount;i++){
		normalArray[i] = 0;
	}
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);

	// Temporary normal vector
	Point3D normal;
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = 
                                tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 5.0;
			//vertexArray[(x + z * tex->width)*3 + 1] = 0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			if(x==0){
                                // Doing something to get rid of warnings
				int a = 0;
                                a += 1;
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
			texCoordArray[(x + z * tex->width)*2 + 0] = 5*(float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = 5*(float)z / tex->height;
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
Model *m, *m2, *tm, *skybox;
// Reference to shader program
GLuint program;
GLuint tex1, tex2, skytex;
TextureData ttex; // terrain

void init(void)
{

        skybox = LoadModelPlus("skybox.obj");


        // GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projectionMatrix);
	
	cam_pos.x = 108.0f;
	cam_pos.y = 60.0f;
	cam_pos.z = 144.0f;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	
	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("dirt.tga", &tex1);

        

        // Init light
        
        Point3D lightSourcesColorsArr = {0.5f, 0.5f, 0.5f}; // Grey light
        GLfloat specularExponent = 2.0;
        GLint isDirectional = 1;
        Point3D lightSourcesDirectionsPositions = {0.5f, 1.0f, 0.5f}; // grey light, positional

        glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 1, &lightSourcesDirectionsPositions.x);
        glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 1, &lightSourcesColorsArr.x);
        glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent);
        glUniform1i(glGetUniformLocation(program, "isDirectional"), isDirectional);

        // Load terrain data
	
	LoadTGATexture("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

        // Load textures	
        LoadTGATextureSimple("SkyBox512.tga", &skytex);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat total[16], modelView[16], camMatrix[16], cam_Matrix_skybox[16];
	
	printError("pre display");
	
	check_keys();

	glUseProgram(program);

	// Build matrix
	
	lookAt(&cam_pos, &obj_pos, up.x, up.y, up.z, camMatrix);
        CopyMatrix(camMatrix, cam_Matrix_skybox);
        cam_Matrix_skybox[3] = 0;
        cam_Matrix_skybox[7] = 0;
        cam_Matrix_skybox[11] = 0;
        cam_Matrix_skybox[15] = 1;
        //disable z-buffer for skybox
        glDisable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //Set the skybox variable 
        glUniform1i(glGetUniformLocation(program, "skybox"), 1);
        T(0,-0.5,0,modelView);
        Mult(cam_Matrix_skybox, modelView, total);
        glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
        glBindTexture(GL_TEXTURE_2D, skytex);
        DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

        glUniform1i(glGetUniformLocation(program, "skybox"), 0);
        glEnable(GL_DEPTH_TEST);
        // Bind terrain
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
                cam_pos.y = calculate_height(cam_pos.x, cam_pos.z, ttex.width, vertexArray) + cam_height;
                VectorAdd(&vdiff, &obj_pos, &obj_pos);
        } else if (keyIsDown('s')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                Normalize(&vdiff);
                VectorSub(&cam_pos, &vdiff, &cam_pos);
                cam_pos.y = calculate_height(cam_pos.x, cam_pos.z, ttex.width, vertexArray) + cam_height;
                VectorSub(&obj_pos, &vdiff, &obj_pos);
        } else if (keyIsDown('a')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                CrossProduct(&up, &vdiff, &vdiff);
                Normalize(&vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                VectorAdd(&vdiff, &cam_pos, &cam_pos);
                cam_pos.y = calculate_height(cam_pos.x, cam_pos.z, ttex.width, vertexArray) + cam_height;
                VectorAdd(&vdiff, &obj_pos, &obj_pos);
        } else if (keyIsDown('d')) {
                //printf("%f, %f, %f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                VectorSub(&obj_pos, &cam_pos, &vdiff);
                CrossProduct(&up, &vdiff, &vdiff);
                Normalize(&vdiff);
                ScalarMult(&vdiff, move_speed, &vdiff);
                VectorSub(&cam_pos, &vdiff, &cam_pos);
                cam_pos.y = calculate_height(cam_pos.x, cam_pos.z, ttex.width, vertexArray) + cam_height;
                VectorSub(&obj_pos, &vdiff, &obj_pos);                                
        } else if (keyIsDown('q')) {
                exit(0);
        } else if (keyIsDown('p')) {
                printf("Your position is; x=%f, y=%f, z=%f\n", cam_pos.x, cam_pos.y, cam_pos.z);
                printf("Ground height is; y=%f\n", calculate_height(cam_pos.x, cam_pos.z, ttex.width, vertexArray));
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
