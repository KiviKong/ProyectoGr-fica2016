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

#define numAsteroids 100
#define maxDepth (-2000)

#define to_rads(a) (180 * a / M_PI)

static GLuint programId, bgProgram, va[3], vertexPosLoc, vertexColLoc, modelMatrixLoc,
		projMatrixLoc, viewMatrixLoc, vertexPosLocBG, vertexColLocBG, modelMatrixLocBG,
		projMatrixLocBG, viewMatrixLocBG, textureLoc;

static Asteroid* asteroids;
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

static short accion = 0;
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

static float cameraX = 0;
static float cameraY = 0;
static float cameraZ = -1;
static float angle = 0;
static float cameraAngle = 0;
static float anglespeed = 1;
static float anglespeed2 = 5;
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
	float randFeo;
	for (i = 0; i < numAsteroids; i++) {
		randVel = (rand() % 5) + 2;
		randRadio = (rand() % 10) + 1;
		randFeo = rand() % 2;
		asteroids[i]=create_asteroid2(randRadio,20,20);
		//asteroids[i] = Asteroid_create(randRadio, 10, 15, randFeo);
		setVelAsteroid(asteroids[i], randVel);

	}

}

static void bindAsteroids() {
	int i;
	for (i = 0; i < numAsteroids; i++) {
		Asteroid_bind(asteroids[i], vertexPosLoc, vertexColLoc);
	}
}

static void drawAsteroids() {
	short collision = 0;
	for (iterator = 0; iterator < numAsteroids; iterator++) {
		if (asteroids[iterator] != NULL) {
			mIdentity(&csMat);
			if ((asteroids[iterator]->z + asteroids[iterator]->speed)
					< cameraZ + 1 - 7
					&& (asteroids[iterator]->z + asteroids[iterator]->speed)
							> cameraZ - 1 - asteroids[iterator]->speed - 7) {
				collision = checkCollision(cameraX+shipX + 1, cameraX+shipX - 1,
						-cameraY-shipY + .8, -cameraY-shipY - .8,
						asteroids[iterator]->x + asteroids[iterator]->r,
						asteroids[iterator]->x - asteroids[iterator]->r,
						asteroids[iterator]->y + asteroids[iterator]->r,
						asteroids[iterator]->y - asteroids[iterator]->r);
				if (collision == 1) {
					extra++;
					printf("%d ", extra);
					printf(" %f, %f, %f, %f, %f, %f, %f, %f ", cameraX + 1,
							cameraX - 1, -cameraY + .8, -cameraY - .8,
							asteroids[iterator]->x + asteroids[iterator]->r,
							asteroids[iterator]->x - asteroids[iterator]->r,
							asteroids[iterator]->y + asteroids[iterator]->r,
							asteroids[iterator]->y - asteroids[iterator]->r);
				}

			}
			translate(&csMat, asteroids[iterator]->x, asteroids[iterator]->y,
					updateAsteroidZ(asteroids[iterator]));

			rotateX(&csMat, angle + 50);
			glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, csMat.values);
			if (collision == 1) {
				Asteroid_destroy(asteroids[iterator]);
				asteroids[iterator] = NULL;
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
// 				SCREEN				   //
// =================================== //

static void createBackground() {
	GLfloat max = -maxDepth * tan(to_rads(53/2));
	printf("%f\n", max);
	background = BackgroundCreate(
		-max, // minX
		max,  // maxX
		-max, // minY
		max,  // maxY
		-1999	 // depth
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
	textureLoc = glGetUniformLocation(bgProgram, "texCoord");
}

static void exitFunc(unsigned char key, int x, int y) {
	if (key == 32) {
		s *= -1;
		cameraAngle += 180 * s;
	}
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

	createBackground();
	createAsteroids();
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
	angleZ += anglespeed;
	if (angleZ > 30)
		angleZ = 30;
	shipX -= 0.05;
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
	angleZ -= anglespeed;
	if (angleZ < -30)
		angleZ = -30;
	shipX += 0.05;
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
	angleY -= anglespeed;
	if (angleY < -30)
		angleY = -30;
	shipY -= 0.05;
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
	angleY += anglespeed;
	if (angleY > 30)
		angleY = 30;
	shipY += 0.05;
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


// =================================== //
// 				SCREEN				   //
// =================================== //

static void display() {

	mIdentity(&csMat);
	Mat4 view;
	mIdentity(&view);
	mIdentity(&shipMat);

	if ((accion & up) != 0) {
		moveUp();
	} else if ((accion & down) != 0) {
		moveDown();
	}
	if ((accion & right) != 0) {
		rotateRight();
	} else if ((accion & left) != 0) {
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

	translate(&shipMat, cameraX, -cameraY, cameraZ);

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


	Mat4 identity;
	mIdentity(&identity);
	glUseProgram(bgProgram);
	glUniformMatrix4fv(projMatrixLocBG, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(modelMatrixLocBG, 1, GL_TRUE,identity.values);
	glUniformMatrix4fv(viewMatrixLocBG, 1, GL_TRUE, identity.values);
	glUniform1i(glGetUniformLocation(bgProgram, "myTexture"), 0);
	BackgroundDraw(background);

	glUseProgram(programId);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, shipMat.values);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, view.values);

	nave_draw(n1);
	drawAsteroids();

	glutSwapBuffers();
}

static void startMotionFunc(int key, int x, int y) {
	motion = key;

	if (key == GLUT_KEY_RIGHT &&(accion&left)!=left) {
		accion |= right;
	} else if (key == GLUT_KEY_LEFT &&(accion&right)!=right) {
		accion |= left;
	} else if (key == GLUT_KEY_DOWN&&(accion&up)!=up) {
		accion |= down;
	} else if (key == GLUT_KEY_UP&&(accion&down)!=down) {
		accion |= up;
	}
}

static void endMotionFunc(int key, int x, int y) {
	if (key == GLUT_KEY_UP) { //arriba es  00000001
		if(correctDown!=1&&(accion&down)!=down)
		correctUp = 1;
		accion &= 0XFE;
	} else if (key == GLUT_KEY_DOWN) { //abajo es  00000010
		if(correctUp!=1&&(accion&up)!=up)
		correctDown = 1;
		accion &= 0XFD;
	} else if (key == GLUT_KEY_RIGHT) { //derecha es  000001000
		if(correctLeft!=1&&(accion&left)!=left)
		correctRight = 1;
		accion &= 0XFB;
	} else { //izquierda es  00001000
		if(correctRight!=1&&(accion&right)!=right)
		correctLeft = 1;
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
