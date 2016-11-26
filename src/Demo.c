/*
 * Demo.c
 *
 *  Created on: 11/10/2016
 *      Author: Rodolfo
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "Utils.h"
#include "Transforms.h"
#include <math.h>
#include <time.h>
#include "Cylinder.h"
#include "Nave.h"
#include "Asteroids.h"
#include "Background.h"
#include "LaserBeam.h"
#include "Cylinder.h"
#include "CylinderStack.h"

#define numAsteroids 2000
#define maxDepth (-2000)
#define numLaser 1

#define X 0
#define Y 1
#define Z 2


#define to_rads(a) (180 * a / M_PI)

static GLuint programId, bgProgram, va[3], vertexPosLoc, vertexColLoc, modelMatrixLoc,
		projMatrixLoc, viewMatrixLoc, vertexNormalLoc, vertexPosLocBG, vertexColLocBG, modelMatrixLocBG,
		projMatrixLocBG, viewMatrixLocBG, textureLoc;


static Asteroid* asteroids;
static CylinderStack stack;
static Background background;
static int iterator;
static Mat4 csMat;
static Nave n1;
static Mat4 projMat;
static Mat4 shipMat;

static int extra = 0;

static float limiteInfY = -80;
static float limiteSY = 80;
static float limiteIX = -80;
static float limiteSX = 80;

static short up = 0x01;
static short down = 0x02;
static short left = 0x08;
static short right = 0x04;
static short acc=0x01;
static short deacc=0x02;

static short accion = 0;
static short accionacc=0;
static short s = 1;
static int motion;
static float speed = 7;
static bool correctUp = 0;
static bool correctDown = 0;
static bool correctRight = 0;
static bool correctLeft = 0;
static float shipX = 0;
static float shipY = 0;
static float shipZ = 0;
static float angleZ = 0;
static float angleY = 0;
static float velocity=1;

static float cameraX = 0;
static float cameraY = 0;
static float cameraZ = -1;
static float angle = 0;
static float cameraAngle = 0;
static float anglespeed = 1;
static float cameraAngleY = 0;
static float aspect;

// =================================== //
// 				ASTEROIDS			   //
// =================================== //
static void createAsteroids() {
	asteroids = (Asteroid*) malloc(sizeof(Asteroid) * numAsteroids);
	int i;
	float randVel;
	float randRadio;
	for (i = 0; i < numAsteroids; i++) {
		randVel = (rand() % 5) + 2;
		randRadio = (rand() % 10) + 1;
		asteroids[i]=create_asteroid2(randRadio,20,20);
		setVelAsteroid(asteroids[i], randVel);
	}

}

static void bindAsteroids() {
	int i;
	for (i = 0; i < numAsteroids; i++) {
		Asteroid_bind(asteroids[i], vertexPosLoc, vertexColLoc,vertexNormalLoc);
	}
}

static void drawAsteroids() {
	short collision = 0;
	for (iterator = 0; iterator < numAsteroids; iterator++) {
		if (asteroids[iterator] != NULL) {
			mIdentity(&csMat);
			if ((asteroids[iterator]->z + asteroids[iterator]->speed < (cameraZ + 1 - 7))
					&& (asteroids[iterator]->z + asteroids[iterator]->speed > (cameraZ - 1 - asteroids[iterator]->speed - 7))) {
				collision = checkCollision(cameraX+shipX + 1, cameraX+shipX - 1,
						-cameraY-shipY + .8, -cameraY-shipY - .8,
						asteroids[iterator]->x + asteroids[iterator]->r,
						asteroids[iterator]->x - asteroids[iterator]->r,
						asteroids[iterator]->y + asteroids[iterator]->r,
						asteroids[iterator]->y - asteroids[iterator]->r);
				if (collision == 1) {
					extra++;
					printf("%d ", extra);
					printf(" %f, %f, %f, %f, %f, %f, %f, %f\n", cameraX + 1,
							cameraX - 1, -cameraY + .8, -cameraY - .8,
							asteroids[iterator]->x + asteroids[iterator]->r,
							asteroids[iterator]->x - asteroids[iterator]->r,
							asteroids[iterator]->y + asteroids[iterator]->r,
							asteroids[iterator]->y - asteroids[iterator]->r);
				}

			}
			translate(&csMat, asteroids[iterator]->x, asteroids[iterator]->y,
					updateAsteroidZ(asteroids[iterator],velocity));

			rotateX(&csMat, angle + 50);
			glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, csMat.values);
			if (collision == 1) {
				Asteroid_destroy(asteroids[iterator]);
				asteroids[iterator] = NULL;
				asteroids[iterator]=create_asteroid2((rand() % 10) + 1,20,20);
				setVelAsteroid(asteroids[iterator], (rand() % 5) + 2);
				Asteroid_bind(asteroids[iterator],vertexPosLoc,vertexColLoc,vertexNormalLoc);

			}
			if (asteroids[iterator] != NULL) {
				Asteroid_draw(asteroids[iterator]);
			}
			collision = 0;
		}
	}

}

static void destroyAsteroids() {
	int i;
	for (i = 0; i < numAsteroids; i++) {
		Asteroid_destroy(asteroids[i]);

	}
}


// =================================== //
// 			  LASER BEAMS			   //
// =================================== //
static void initLaserBeams() {
	stack = Stack_create();
}

static void shootNewLaser() {
	float col[] = {1, 1, 1};
	float dx = (accion & left) ? -speed : (accion & right) ? speed : 0;
	float dy = (accion & up) ? -speed : (accion & down) ? speed : 0;
	Cylinder new = cylinder_create (
		10, 				// float length
		0.2, 				// float bottomRadius
		0.2, 				// float topRadius
		10,					// int slices
		1,					// int stacks
		col,				// float bottomColor[3]
		col,				// float topColor[3]
		(shipX + cameraX + dx),	// float coordX
		(shipY - cameraY + dy),	// float coordY
		(shipZ - 10)		// float coordZ
	);
	cylinder_bind(new, vertexPosLoc, vertexColLoc);
	push(stack, new);
}


// =================================== //
// 				SCREEN				   //
// =================================== //

static void createBackground() {
	GLfloat max = -maxDepth * tan(to_rads(53/2));
	background = BackgroundCreate(
		-max*1.5, // minX
		max*1.5,  // maxX
		-max*1.5, // minY
		max*1.5,  // maxY
		-1800	 // depth
	);
	BackgroundBind(background, vertexPosLocBG, vertexColLocBG, textureLoc);
}


// =================================== //
// 				SCREEN				   //
// =================================== //

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
	vertexNormalLoc=glGetAttribLocation(programId,"vertexNormal");

	GLuint bgShader = compileShader("shaders/bg.fsh", GL_FRAGMENT_SHADER);
	if (!shaderCompiled(bgShader))
		return;

	bgProgram = glCreateProgram();
	glAttachShader(bgProgram, vShader);
	glAttachShader(bgProgram, bgShader);
	glLinkProgram(bgProgram);
	vertexPosLocBG = glGetAttribLocation(bgProgram, "vertexPosition");
	vertexColLocBG = glGetAttribLocation(bgProgram, "vertexColor");
	modelMatrixLocBG = glGetUniformLocation(bgProgram, "modelMatrix");
	projMatrixLocBG = glGetUniformLocation(bgProgram, "projMatrix");
	viewMatrixLocBG = glGetUniformLocation(bgProgram, "viewMatrix");
	textureLoc = glGetAttribLocation(bgProgram, "texCoord");
}

static void exitFunc(unsigned char key, int x, int y) {
	switch (key) {
		case 13:
			s *= -1;
			cameraAngle += 180 * s;
			break;
		case 32:
			printf("=========\nENTER: (%f, %f, %f)\n=========\n", (shipX + cameraX), (shipY + cameraY), (shipZ + cameraZ));
			shootNewLaser();
			break;
		case 27:
			glDeleteVertexArrays(1, va);
			exit(0);
			break;
	}
}

static void timerFunc(int id) {
	glutTimerFunc(20, timerFunc, id);
	glutPostRedisplay();
}

static void createShape() {

	createBackground();
	createAsteroids();
	initLaserBeams();
	bindAsteroids();

	n1 = nave_create();
	nave_bind(n1, vertexPosLoc, vertexColLoc);

}

static void reshapeFunc(int width, int height) {
	aspect = (float) width / height;
	setPerspective(&projMat, 53, aspect, -1, maxDepth);

	glViewport(0, 0, width, height);
}


// =================================== //
// 				MOVEMENT			   //
// =================================== //

static void rotateLeft() {
	cameraAngle += anglespeed;
	/*if (cameraAngle > 60*s)
	 cameraAngle = 60*s;*/
	angleZ += anglespeed+((velocity-1)*.05);
	if (angleZ > 30)
		angleZ = 30;
	shipX -= 0.05+((velocity-1)*.03);
	if (shipX < -1.7)
		shipX = -1.7;
	if (cameraAngle > 20) {
		cameraAngle = 20;
	}
	if(shipX+cameraX<limiteIX){
		cameraAngle-=anglespeed*2;
		if(cameraAngle<0)cameraAngle=0;
		shipX+=0.10;
		if(shipX>0)shipX=0;
		angleZ-=anglespeed*2;
		if(angleZ<0)angleZ=0;
	}
	correctLeft = 0;
}

static void rotateRight() {
	cameraAngle -= anglespeed;
	/*if (cameraAngle < -60*s)
	 cameraAngle = -60*s;*/
	angleZ -= anglespeed+((velocity-1)*.05);
	if (angleZ < -30)
		angleZ = -30;
	shipX += 0.05+((velocity-1)*.03);
	if (shipX > 1.7)
		shipX = 1.7;

	if (cameraAngle < -20) {
		cameraAngle = -20;
	}
	if(shipX+cameraX>limiteSX){
		cameraAngle+=anglespeed*2;
		if(cameraAngle>0)cameraAngle=0;
		shipX-=0.10;
		if(shipX<0)shipX=0;
		angleZ+=anglespeed*2;
		if(angleZ>0)angleZ=0;
	}
	correctRight = 0;
}

static void moveForward() {
	cameraX -= sin((cameraAngle) * (M_PI / 180))
			* (cos((cameraAngleY) * (M_PI / 180))) * speed;
	if (cameraX > limiteSX)
		cameraX = limiteSX;
	if (cameraX < limiteIX)
		cameraX = limiteIX;

	/*cameraZ -= cos((cameraAngle) * (M_PI / 180))
	 * (cos((cameraAngleY) * (M_PI / 180))) * speed;*/
	cameraY += sin((cameraAngleY) * (M_PI / 180)) * speed;
	if (cameraY > limiteSY)
		cameraY = limiteSY;
	if (cameraY < limiteInfY)
		cameraY = limiteInfY;
}

static void moveUp() {
	cameraAngleY += anglespeed;
	if (cameraAngleY > 20)
		cameraAngleY = 20;
	angleY -= anglespeed+((velocity-1)*.05);
	if (angleY < -30)
		angleY = -30;
	shipY -= 0.05+((velocity-1)*.03);
	if (shipY < -1.7)
		shipY = -1.7;
	if (shipY - cameraY < limiteInfY) {
		cameraAngleY-=anglespeed*2;
		if(cameraAngleY<0)cameraAngleY=0;
		shipY += 0.1;
		if(shipY>0)shipY=0;
		angleY += anglespeed * 2;
		if(angleY>0)angleY=0;
	}

	correctUp = 0;
}

static void moveDown() {
	cameraAngleY -= anglespeed;
	angleY += anglespeed+((velocity-1)*.05);
	if (angleY > 30)
		angleY = 30;
	shipY += 0.05+((velocity-1)*.03);
	if (shipY > 1.7)
		shipY = 1.7;
	if (cameraAngleY < -20)
		cameraAngleY = -20;
	if (shipY - cameraY > limiteSY) {
		cameraAngleY+=anglespeed*2;
		if(cameraAngleY>0)cameraAngleY=0;
		shipY -= 0.1;
		if(shipY<0)shipY=0;
		angleY -= anglespeed * 2;
		if(angleY<0)angleY=0;
	}

	correctDown = 0;
}
static void accelerate(){
	velocity+=0.1;
	if(velocity>1.8)
			velocity=1.8;
}

static void stop(){
	velocity-=0.1;
	if(velocity<0.5)
		velocity=0.5;
}
static void resetVelocity(){
	printf("%f ",velocity);
	if(velocity>1){
		velocity-=0.1;
		if(velocity<1)
			velocity=1;
	}else if(velocity<1){
		velocity+=0.1;
		if(velocity>1)
			velocity=1;
	}
}


// =================================== //
// 				SCREEN				   //
// =================================== //

static void display() {

	mIdentity(&csMat);
	Mat4 view;
	mIdentity(&view);
	mIdentity(&shipMat);

	if((accionacc & acc)!=0){
		accelerate();
	}if((accionacc & deacc)!=0)
		stop();
	if((accionacc & acc)==0 &&(accionacc & deacc)==0&& velocity!=1.0)
		resetVelocity();

	if ((accion & up) != 0) {
		moveUp();
	} if ((accion & down) != 0) {
		moveDown();
	}
	if ((accion & right) != 0) {
		rotateRight();
	}if ((accion & left) != 0) {
		rotateLeft();
	}

	if (correctUp) {
		cameraY -= 0.05;
		shipY += 0.05;
		angleY += anglespeed;
		cameraAngleY -= anglespeed;
		if (cameraAngleY < 0) {
			cameraAngleY = 0;
		}
		if (angleY >= 0) {
			angleY = 0;
		}
		if (shipY >= 0) {
			shipY = 0;
			cameraY += 0.05;
		}
		if (angleY == 0 && shipY == 0 && cameraAngleY == 0) {
			correctUp = 0;
		}
	} else if (correctDown) {
		cameraY += 0.05;
		shipY -= 0.05;
		angleY -= anglespeed;
		cameraAngleY += anglespeed;
		if (cameraAngleY > 0) {
			cameraAngleY = 0;
		}
		if (angleY <= 0) {
			angleY = 0;
		}
		if (shipY <= 0) {
			shipY = 0;
			cameraY -= 0.05;
		}
		if (angleY == 0 && shipY == 0 && cameraAngleY == 0) {
			correctDown = 0;
		}
	}
	if (correctRight) {
		cameraX += 0.05;
		shipX -= 0.05;
		angleZ += anglespeed;
		cameraAngle += anglespeed;
		if (cameraAngle > 0) {
			cameraAngle = 0;
		}
		if (shipX <= 0) {
			shipX = 0;
			cameraX -= 0.05;
		}
		if (angleZ >= 0) {
			angleZ = 0;
		}
		if (shipX == 0 && angleZ == 0 && cameraAngle == 0) {
			correctRight = 0;
		}
	} else if (correctLeft) {
		cameraX -= 0.05;
		shipX += 0.05;
		angleZ -= anglespeed;
		cameraAngle -= anglespeed;
		if (cameraAngle < 0) {
			cameraAngle = 0;
		}
		if (angleZ <= 0) {
			angleZ = 0;
		}
		if (shipX >= 0) {
			shipX = 0;
			cameraX += 0.05;
		}
		if (shipX == 0 && angleZ == 0 && cameraAngle == 0) {
			correctLeft = 0;
		}
	}

	moveForward();
	rotateX(&view, cameraAngleY);
	rotateY(&view, -cameraAngle);

	translate(&view, -cameraX, cameraY, -cameraZ);

	translate(&shipMat, cameraX, -cameraY, cameraZ-((velocity-1)*3));

	rotateY(&shipMat, cameraAngle);
	rotateX(&shipMat, -cameraAngleY);

	translate(&shipMat, (sin(cameraAngle)) * 0.001 * sin(cameraAngleY),
			-cos(cameraAngleY) * .001, (-cos(cameraAngle)) * 0.001 - 7);

	translate(&shipMat, shipX, shipY, shipZ);
	rotateZ(&shipMat, angleZ);
	rotateX(&shipMat, angleY);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	translate(&csMat, -3, 0, -9);

	if (angle == 360)
		angle = 0;
	rotateY(&csMat, angle++);
	rotateX(&csMat, angle);

	//Prueba camara
	/*translate(&shipMat,0,1,0);
	translate(&csMat,0,1,0);
	rotateX(&shipMat,-15);
	rotateX(&csMat,-15);
*/
	//fin prueba camara

	Mat4 identity;
	mIdentity(&identity);
	glUseProgram(bgProgram);
	glUniformMatrix4fv(projMatrixLocBG, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(modelMatrixLocBG, 1, GL_TRUE,identity.values);
	glUniformMatrix4fv(viewMatrixLocBG, 1, GL_TRUE, view.values);
	// glUniform1i(glGetUniformLocation(bgProgram, "myTexture"), 0);
	BackgroundDraw(background);

	glUseProgram(programId);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, view.values);

	Mat4 laserMat;
	int i;
	for(i = 0; i < stack->top; i++) {
		Cylinder tmp = stack->stk[i];
		mIdentity(&laserMat);
		translate(&laserMat, tmp->coord[X], tmp->coord[Y], tmp->coord[Z]--);
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, laserMat.values);
		cylinder_draw(tmp);
	}

	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, shipMat.values);
	nave_draw(n1);
	drawAsteroids();

	glutSwapBuffers();
}

static void startMotionFunc(int key, int x, int y) {
	motion = key;

	if(key==GLUT_KEY_SHIFT_L||key=='A'){
		accionacc |=acc;
		//accionacc &= 0XFD;
	}if(key==GLUT_KEY_SHIFT_R||key=='S'){
		accionacc |=deacc;
		//accionacc &= 0XFE;
	}
	if (key == GLUT_KEY_RIGHT ) {
		accion |= right;
		correctLeft=0;
		correctRight=0;

	} else if (key == GLUT_KEY_LEFT ) {
		accion |= left;
		correctRight=0;
		correctLeft=0;

	} else if (key == GLUT_KEY_DOWN) {
		accion |= down;
		correctUp=0;
		correctDown=0;

	} else if (key == GLUT_KEY_UP) {
		accion |= up;
		correctDown=0;
		correctUp=0;

	}
}

static void endMotionFunc(int key, int x, int y) {
	if(key==GLUT_KEY_SHIFT_L)
		accionacc &= 0XFE;
	if(key==GLUT_KEY_SHIFT_R)
		accionacc &= 0XFD;

	if (key == GLUT_KEY_UP) { //arriba es  00000001
		if((accion&down)!=down){
		if(shipY<0){
			correctUp = 1;
		}else if(shipY>0){
			correctDown = 1;
		}
		}
		accion &= 0XFE;
	} else if (key == GLUT_KEY_DOWN) { //abajo es  00000010
		if((accion&up)!=up){
		if(shipY<0){
			correctUp = 1;
		}else if(shipY>0){
			correctDown = 1;
		}
	}
		accion &= 0XFD;
	} else if (key == GLUT_KEY_RIGHT) { //derecha es  000001000
		if((accion&left)!=left){
		if(shipX<0){
			correctLeft = 1;
		}else if(shipX>0){
			correctRight = 1;
		}
		}
		accion &= 0XFB;
	} else { //izquierda es  00001000
		if((accion&right)!=right){
		if(shipX<0){
			correctLeft = 1;
		}else if(shipX>0){
			correctRight = 1;
		}
		}
		accion &= 0XF7;
	}

	motion = 0;
}


// =================================== //
// 				MAIN				   //
// =================================== //

int main(int argc, char **argv) {
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Demo proyecto final");
	glutDisplayFunc(display);
	glutKeyboardFunc(exitFunc);
	glutReshapeFunc(reshapeFunc);
	glutSpecialFunc(startMotionFunc);
	glutSpecialUpFunc(endMotionFunc);
	glewInit();
	initShaders();
	createShape();
	glutMainLoop();

	destroyAsteroids();
	nave_destroy(n1);
	free(asteroids);
	return 0;
}
