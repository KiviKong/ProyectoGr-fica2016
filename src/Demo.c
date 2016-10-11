/*
 * Demo.c
 *
 *  Created on: 11/10/2016
 *      Author: Rodolfo
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include "Transforms.h"
#include <math.h>
#include "Cylinder.h"
#include "Nave.h"

static GLuint programId, va[3], vertexPosLoc, vertexColLoc, modelMatrixLoc,
		projMatrixLoc, viewMatrixLoc;



static Cylinder c1;
static Cylinder c2;
static Cylinder c3;
static Nave n1;
static Mat4 projMat;
static Mat4 shipMat;

static int motion;
static float speed = 1;

static float cameraX = 0;
static float cameraZ = 0;
static float angle = 0;
static float cameraAngle = 0;
static float anglespeed=3;

static void initShaders() {
	GLuint vShader = compileShader("shaders/projMatrix.vsh", GL_VERTEX_SHADER);
	if (!shaderCompiled(vShader))
		return;
	GLuint fShader = compileShader("shaders/color_n.fsh", GL_FRAGMENT_SHADER);
	if (!shaderCompiled(fShader))
		return;

	programId = glCreateProgram();
	glAttachShader(programId, vShader);
	glAttachShader(programId, fShader);
	glLinkProgram(programId);
	vertexPosLoc = glGetAttribLocation(programId, "vertexPosition");
	vertexColLoc = glGetAttribLocation(programId, "vertexColor");
	modelMatrixLoc = glGetUniformLocation(programId, "modelMatrix");
	projMatrixLoc = glGetUniformLocation(programId, "projMatrix");
	viewMatrixLoc = glGetUniformLocation(programId, "viewMatrix");
}

static void exitFunc(unsigned char key, int x, int y) {
	if (key == 27) {
		glDeleteVertexArrays(1, va);
		exit(0);
	}
}

static void timerFunc(int id) {
	glutTimerFunc(20, timerFunc, id);
	glutPostRedisplay();
}

static void createShape() {
	float color1[] = { 0.9, 0.5, 0.7 };
	float color2[] = { 0.3, 0.1, 0.2 };
	//c2 = cylinder_create(2, 2, 2, 4, 3, color2, color1);

	c1 = cylinder_create(5, 2, 1, 6, 12, color1, color2);
	/*c3 = cylinder_create(7,3,0,20,20,color1,color2);
	glUseProgram(programId);
	*/

	cylinder_bind(c1, vertexPosLoc, vertexColLoc);
	/*cylinder_bind(c2, vertexPosLoc, vertexColLoc);
	cylinder_bind(c3,vertexPosLoc,vertexColLoc);*/

	n1=nave_create();
	nave_bind(n1,vertexPosLoc,vertexColLoc);

	//printarray(c1);

}

static void reshapeFunc(int width, int height) {
	float aspect = (float) width / height;
	setPerspective(&projMat, 53, aspect, -1, -100);
/*
	if (width > height)
		setOrtho(&projMat, -3 * aspect, 3 * aspect, -3, 3, -3, 3);
	else
		setOrtho(&projMat, -3, 3, -3 / aspect, 3 / aspect, -3, 3);*/

	glViewport(0, 0, width, height);
}

static void rotateLeft() {
	cameraAngle+=anglespeed;
}
static void rotateRight() {
	cameraAngle-=anglespeed;
}
static void moveForward() {
	cameraX -= sin((cameraAngle) * (M_PI / 180)) * speed;
	cameraZ -= cos((cameraAngle) * (M_PI / 180)) * speed;
}

static void moveBackwards() {
	cameraX += sin((cameraAngle) * (M_PI / 180)) * speed;
	cameraZ += cos((cameraAngle) * (M_PI / 180)) * speed;
}

static void display() {
	Mat4 csMat;
	mIdentity(&csMat);
	Mat4 view;
	mIdentity(&view);
	mIdentity(&shipMat);
	switch (motion) {
		case 1:
			break;
		case GLUT_KEY_RIGHT:
			rotateRight();
			break;
		case GLUT_KEY_LEFT:
			rotateLeft();
			break;
		/*case GLUT_KEY_UP:
			moveForward();
			break;
		case GLUT_KEY_DOWN:
			moveBackwards();
			break;*/
		default:
			break;
		}
	moveForward();
	rotateY(&view,-cameraAngle);

	translate(&view,-cameraX,0,-cameraZ);


	translate(&shipMat,cameraX,0,cameraZ);
	rotateY(&shipMat,cameraAngle);
	translate(&shipMat,(sin(cameraAngle))*0.001,0,(-cos(cameraAngle))*0.001-7);



	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	translate(&csMat, -3, 0, -9);
	//rotateX(&csMat,90);
	if (angle == 360)
		angle = 0;
	rotateY(&csMat, angle++);
	rotateX(&csMat, angle);
	glUseProgram(programId);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, shipMat.values);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, view.values);

	/*cylinder_draw(c1);
	cylinder_draw(c2);
	cylinder_draw(c3);*/
	nave_draw(n1);
	int i=0;
	int z=0;
	for( i=0;i<10000;i++){
	mIdentity(&csMat);
	if(i%2==0){
	translate(&csMat, 3, 0, z-=10);
	}else
		translate(&csMat,-3,0,z-=10);
	//rotateX(&csMat, angle+50);
	//rotateY(&csMat, angle+50);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, csMat.values);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, view.values);
	cylinder_draw(c1);
	}
	/*cylinder_draw(c2);
	cylinder_draw(c1);
	cylinder_draw(c3);*/
	//nave_draw(n1);

	glutSwapBuffers();
}

static void startMotionFunc(int key, int x, int y) {
	motion = key;
	/*
	 if(key==GLUT_KEY_RIGHT){

	 }else if(key==GLUT_KEY_LEFT){

	 }else if(key==GLUT_KEY_DOWN){

	 }else if(key==GLUT_KEY_UP){

	 }*/
}
static void endMotionFunc(int key, int x, int y) {
	motion = 0;
}
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Tarea 5");
	glutDisplayFunc(display);
	glutKeyboardFunc(exitFunc);
	glutReshapeFunc(reshapeFunc);
	glutSpecialFunc(startMotionFunc);
	glutSpecialUpFunc(endMotionFunc);
	glewInit();
	initShaders();
	createShape();
	//printf("%f",c1->vertexPos);
	//display();
	glClearColor(0.05, 0.05, 0.10, 1.0);
	glutMainLoop();

	cylinder_destroy(c1);
	return 0;
}

