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
#include "Numbers.h"

#define numAsteroids 2000
#define maxDepth (-2000)
#define numLaser 1

#define X 0
#define Y 1
#define Z 2

#define log(a) printf("Log: %d\n", a);
#define logs(s) printf("Log: %s\n", s);

#define to_rads(a) (180 * a / M_PI)

typedef float vec3[3];
typedef struct {
	float x, y, z, w;
} Vec4;

static GLuint programId, lightProgramId, bgProgram, va[3], vertexPosLoc, vertexColLoc, modelMatrixLoc,
		projMatrixLoc, viewMatrixLoc, vertexNormalLoc, vertexPosLocBG, vertexColLocBG, modelMatrixLocBG,
		projMatrixLocBG, viewMatrixLocBG, vertexPosLocIl, vertexColLocIl, modelMatrixLocIl,
		projMatrixLocIl, viewMatrixLocIl,textureLoc,vertexNormalLocIl,textureLocIl;
static GLuint ambientLightLoc, materialALoc, materialDLoc;
static GLuint materialSLoc, cameraPositionLoc;

static vec3 ambientLight = { 0.9, 0.9, 0.9 };
static vec3 materialA = { 0.9, 0.9, 0.9 };
static vec3 materialD = { 0.4, 0.4, 0.4 };
static vec3 materialS = { 0.2, 0.2, 0.2 };

static float shipLightX = 0;
static float shipLightY = 0;
static float shipLightZ = 0;

//                          Color    Pad  Position  exponent  direction cutoff	subcutoff  padding
static float lights[] = {
	1, 0.98, 0.7,	// Color
	0,				// Padding
	-500, 0, -15,	// Position
	128,			// Exponent
	0, 0, -1,		// Direction
	0.05,			// Cutoff
	0.2,			// Subcutoff
	0.0,
	0.0, 0.0, 1, 1, 1, 0, 0, 0, 0, 8, 0, 0, -1, 0.95, 0.99, 0.0, 0.0,
	0.0, 0, 1, 1, 1, 500, 0, -15, 128, 0, 0, -1, 0.7071, 0.92, 0.0, 0.0, 0.0 };
static GLuint lightsBufferId;

static Asteroid* asteroids;
static CylinderStack stack;
static Background background;
static Background titleScreen;
static Background gameOverScreen;
static Numbers number;
static int iterator;
static Mat4 csMat;
static Nave n1;
static Mat4 projMat;
static Mat4 shipMat;
static Mat4 laserMat;
static Mat4 view;

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
static float laserSpeed=-2.0;

static bool shoot=0;
static float laserX=0;
static float laserY=0;
static float laserZ=0;

static float cameraX = 0;
static float cameraY = 0;
static float cameraZ = -1;
static float angle = 0;
static float cameraAngle = 0;
static float anglespeed = 1;
static float cameraAngleY = 0;
static float aspect;

static bool gameOver=0;
static bool startGame=0;

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
		//printf("create %d",i);
	}

}

static void bindAsteroids() {
	int i;
	for (i = 0; i < numAsteroids; i++) {
		Asteroid_bind(asteroids[i], vertexPosLocIl, vertexColLocIl,vertexNormalLocIl);
	}
}

static void drawAsteroids() {

	short collision = 0;
	for (iterator = 0; iterator < numAsteroids; iterator++) {
		if (asteroids[iterator] != NULL) {
			mIdentity(&csMat);
			if(!asteroids[iterator]->shot)
			if ((asteroids[iterator]->z + asteroids[iterator]->speed < (cameraZ + 1 - 7-((velocity-1)*3)))
					&& (asteroids[iterator]->z + asteroids[iterator]->speed > (cameraZ - 1 - asteroids[iterator]->speed - 7-((velocity-1)*3)))) {
				collision = checkCollision(cameraX+shipX + 1, cameraX+shipX - 1,
						-cameraY-shipY + .8, -cameraY-shipY - .8,
						asteroids[iterator]->x + asteroids[iterator]->r,
						asteroids[iterator]->x - asteroids[iterator]->r,
						asteroids[iterator]->y + asteroids[iterator]->r,
						asteroids[iterator]->y - asteroids[iterator]->r);
				/*if (collision == 1) {
					extra++;
					printf("%d ", extra);
					printf(" %f, %f, %f, %f, %f, %f, %f, %f\n", cameraX + 1,
							cameraX - 1, -cameraY + .8, -cameraY - .8,
							asteroids[iterator]->x + asteroids[iterator]->r,
							asteroids[iterator]->x - asteroids[iterator]->r,
							asteroids[iterator]->y + asteroids[iterator]->r,
							asteroids[iterator]->y - asteroids[iterator]->r);
				}*/

			}
			translate(&csMat, asteroids[iterator]->x, asteroids[iterator]->y,
					updateAsteroidZ(asteroids[iterator],velocity));

			rotateX(&csMat, angle + 50);
			if(asteroids[iterator]->shot){
				if(asteroids[iterator]->scale>0)
				asteroids[iterator]->scale-=0.1;
				scale(&csMat,asteroids[iterator]->scale,asteroids[iterator]->scale,asteroids[iterator]->scale);
			}
			glUniformMatrix4fv(modelMatrixLocIl, 1, GL_TRUE, csMat.values);
			if (collision == 1) {
				n1->hp--;
				Asteroid_destroy(asteroids[iterator]);
				asteroids[iterator] = NULL;
				asteroids[iterator]=create_asteroid2((rand() % 10) + 1,20,20);
				setVelAsteroid(asteroids[iterator], (rand() % 5) + 2);
				Asteroid_bind(asteroids[iterator],vertexPosLocIl,vertexColLocIl,vertexNormalLocIl);
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
		log(i)
		Asteroid_destroy(asteroids[i]);
		asteroids[i]=NULL;
	}
	free(asteroids);
}


// =================================== //
// 			  LASER BEAMS			   //
// =================================== //
static void initLaserBeams() {
	stack = Stack_create();
	//printf("createbeams");
}

static void shootNewLaser(float posX,float posY,float posZ,float velX,float velY, float velZ) {
	float col[] = {1, 1, 1};
	float dx = (accion & left) ? -speed : (accion & right) ? speed : 0;
	float dy = (accion & up) ? -speed : (accion & down) ? speed : 0;
	if(cameraX>=limiteSX || cameraX<=limiteIX)
		dx=0;
	if(cameraY>=limiteSY || cameraY<=limiteInfY)
		dy=0;

	Cylinder new = cylinder_create (
		10, 				// float length
		0.2, 				// float bottomRadius
		0.2, 				// float topRadius
		10,					// int slices
		1,					// int stacks
		col,				// float bottomColor[3]
		col,				// float topColor[3]
		posX+velX+(dx*.5),	// float coordX
		posY+velY+(dy*.5),	// float coordY
		(posZ - 15),		// float coordZ
		velX,						//velocidadX
		velY,						//velocidadY
		velZ						//velocidadZ
	);
	cylinder_bind(new, vertexPosLoc, vertexColLoc);
	push(stack, new);
}

static void drawLaserBeams() {
	int i;
	bool collided = false;
	int idColl = 0;
	for(i = 0; i < 25; i++) {//25 pornumero maximo de disparos segun stack

		if(stack->stk[i] != NULL) {
			Cylinder tmp = stack->stk[i];
			if(tmp->coord[Z] < -200) {
				stack->top--;
				stack->stk[i]=NULL;
				cylinder_destroy(tmp);
			} else {
				for (iterator = 0; iterator < numAsteroids; iterator++) {
					if(asteroids[iterator] != NULL)
						if(!asteroids[iterator]->shot)
						if((asteroids[iterator]->z + (asteroids[iterator]->speed*velocity) < (tmp->coord[2] + 5 - (tmp->velZ/(velocity*1.0))))
								&& (asteroids[iterator]->z + (asteroids[iterator]->speed*velocity) > (tmp->coord[2] - 5 - (asteroids[iterator]->speed*velocity) - (tmp->velZ/(velocity*1.0)))))
							collided = Asteroid_collide(asteroids[iterator], tmp);
					if(collided) {
						idColl = iterator;
						break;
					}
				}
				if(!collided) {
					mIdentity(&laserMat);
					translate(&laserMat, tmp->coord[X]+=(tmp->velX/(velocity*1.0)), tmp->coord[Y]+=(tmp->velY/(velocity*1.0)), tmp->coord[Z]+=(tmp->velZ/(velocity*1.0)));

					rotateY(&laserMat,(atan((tmp->velX/5)/(tmp->velZ/4)))/M_PI);
					rotateX(&laserMat,-(atan((tmp->velY/5)/(tmp->velZ/4)))/M_PI);

					glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, laserMat.values);
					cylinder_draw(tmp);
				} else {
					//printf("colision con asteroide");
					cylinder_destroy(tmp);
					stack->stk[i]=NULL;
					//printf("asteroidhp %d\n",asteroids[idColl]->hp);
					asteroids[idColl]->hp-=1;
					//printf("asteroidhp %d\n",asteroids[idColl]->hp);
					//if(asteroids[idColl]->hp<=0){
					asteroids[idColl]->shot=true;
					n1->puntuacion+=asteroids[idColl]->r*5;
						/*
					Asteroid_destroy(asteroids[idColl]);
					asteroids[idColl] = NULL;
					asteroids[idColl]=create_asteroid2((rand() % 10) + 1,20,20);
					setVelAsteroid(asteroids[idColl], (rand() % 5) + 2);
					Asteroid_bind(asteroids[idColl],vertexPosLocIl,vertexColLocIl,vertexNormalLocIl);*/
					collided=0;
					//}
				}
			}
		}
	}
}

static void destroyLaserBeams() {
	Stack_destroy(stack);
	printf("destroyedbeams");
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
		-1800,	 // depth
		0
	);
	BackgroundBind(background, vertexPosLocBG, vertexColLocBG, textureLoc,0);

	titleScreen = BackgroundCreate(
			-max*1.5, // minX
			max*1.5,  // maxX
			-max*1.5, // minY
			max*1.5,  // maxY
			-1800,	 // depth
			1
	);
	BackgroundBind(titleScreen, vertexPosLocBG, vertexColLocBG, textureLoc,1);

		gameOverScreen = BackgroundCreate(
				-max*1.5, // minX
				max*1.5,  // maxX
				-max*1.5, // minY
				max*1.5,  // maxY
				-1800,	 // depth
				1
		);
		BackgroundBind(gameOverScreen, vertexPosLocBG, vertexColLocBG, textureLoc,2);

	number=createNumbers();
	numberBind(number, vertexPosLocBG, vertexColLocBG, textureLoc);
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

	GLuint IlvShader=compileShader("shaders/phong_1.vsh",GL_VERTEX_SHADER);
	if (!shaderCompiled(IlvShader))
			return;
	GLuint IlfShader=compileShader("shaders/phong_1.fsh",GL_FRAGMENT_SHADER);
	if (!shaderCompiled(IlfShader))
			return;
	lightProgramId = glCreateProgram();
	glAttachShader(lightProgramId, IlvShader);
	glAttachShader(lightProgramId, IlfShader);
	glLinkProgram(lightProgramId);
	vertexPosLocIl = glGetAttribLocation(lightProgramId, "vertexPosition");
	vertexColLocIl = glGetAttribLocation(lightProgramId, "vertexColor");
	modelMatrixLocIl = glGetUniformLocation(lightProgramId, "modelMatrix");
	projMatrixLocIl = glGetUniformLocation(lightProgramId, "projMatrix");
	viewMatrixLocIl = glGetUniformLocation(lightProgramId, "viewMatrix");
	vertexNormalLocIl=glGetAttribLocation(lightProgramId,"vertexNormal");
	textureLocIl = glGetAttribLocation(lightProgramId, "vertexTextCoord");
	ambientLightLoc = glGetUniformLocation(lightProgramId, "ambientLight");
	materialALoc = glGetUniformLocation(lightProgramId, "materialA");
	materialDLoc = glGetUniformLocation(lightProgramId, "materialD");
	materialSLoc = glGetUniformLocation(lightProgramId, "materialS");
	cameraPositionLoc = glGetUniformLocation(lightProgramId, "cameraPosition");

}
static void initLight() {
	glUseProgram(lightProgramId);
	glUniform3fv(ambientLightLoc, 1, ambientLight);

	glUniform3fv(materialALoc, 1, materialA);
	glUniform3fv(materialDLoc, 1, materialD);
	glUniform3fv(materialSLoc, 1, materialS);

	glGenBuffers(1, &lightsBufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsBufferId);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), lights, GL_DYNAMIC_DRAW);

	GLuint uniformBlockIndex = glGetUniformBlockIndex(lightProgramId, "LightBlock");
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsBufferId);
	glUniformBlockBinding(lightProgramId, uniformBlockIndex, 0);
}

static void startKey(unsigned char key, int x, int y) {
	switch (key) {
	case 70:
	case 102:
		if(gameOver){
			printf("final score: %d \n",n1->puntuacion);
		}
		break;
	case 82:
	case 114:
		if(gameOver){
			gameOver=false;
			n1->hp=5;
			n1->puntuacion=0;
			startGame=true;
			int i;
			for(i=0;i<numAsteroids;i++){
				resetAsteroidZ(asteroids[i]);
			}
		}

		break;
		case 13:
			if(!gameOver)
			startGame=true;
			break;
		case 32:
			accionacc |=acc;
			break;
		case 27:
			glDeleteVertexArrays(1, va);
			exit(0);
			break;
		case 65://a A
		case 97:
			accion |= left;
			correctRight=0;
			correctLeft=0;
			break;
		case 87://w W
		case 119:
			accion |= up;
			correctDown=0;
			correctUp=0;
			break;
		case 68://d D
		case 100:
			accion |= right;
			correctLeft=0;
			correctRight=0;
			break;
		case 83://s S
		case 115:
			accion |= down;
			correctUp=0;
			correctDown=0;

			break;
	}
}

static void endKey(unsigned char key, int x, int y){
	if(key==32)
		accionacc &= 0XFE;

	if (key == 87 || key==119) { //arriba es  00000001
			if((accion&down)!=down){
			if(shipY<0){
				correctUp = 1;
			}else if(shipY>0){
				correctDown = 1;
			}
			}
			accion &= 0XFE;
		} else if (key == 83 || key==115) { //abajo es  00000010
			if((accion&up)!=up){
			if(shipY<0){
				correctUp = 1;
			}else if(shipY>0){
				correctDown = 1;
			}
		}
			accion &= 0XFD;
		} else if (key == 68 ||key==100) { //derecha es  000001000
			if((accion&left)!=left){
			if(shipX<0){
				correctLeft = 1;
			}else if(shipX>0){
				correctRight = 1;
			}
			}
			accion &= 0XFB;
		} else if (key == 65 ||key==97) { //izquierda es  00001000
			if((accion&right)!=right){
			if(shipX<0){
				correctLeft = 1;
			}else if(shipX>0){
				correctRight = 1;
			}
			}
			accion &= 0XF7;
		}
}
static void matXvec(Mat4 m, Vec4 v, Vec4* r){
	r->x = (v.x * m.at[0][0]) + (v.y * m.at[0][1]) +
			(v.z * m.at[0][2]) + (v.w * m.at[0][3]);
	r->y = (v.x * m.at[1][0]) + (v.y * m.at[1][1]) +
				(v.z * m.at[1][2]) + (v.w * m.at[1][3]);
	r->z = (v.x * m.at[2][0]) + (v.y * m.at[2][1]) +
				(v.z * m.at[2][2]) + (v.w * m.at[2][3]);
	r->w = (v.x * m.at[3][0]) + (v.y * m.at[3][1]) +
				(v.z * m.at[3][2]) + (v.w * m.at[3][3]);
}

static void normalize(Vec4* v) {
	float magnitude = sqrt(
			pow(v->x, 2) +
			pow(v->y, 2) +
			pow(v->z, 2) +
			pow(v->w, 2)
		);
	v->x /= magnitude;
	v->y /= magnitude;
	v->z /= magnitude;
	v->w /= magnitude;
}

static void mouseClick(int button, int state, int mx, int my){
	if(state != GLUT_UP) return;

		// ************************
		// AQUÍ ESTÁ EL TRABAJO MÁS PESADO

		float width = glutGet(GLUT_WINDOW_WIDTH);
		float height = glutGet(GLUT_WINDOW_HEIGHT);

		// Obtener coordenadas de dispositivo normalizado
		float Pnx = (2 * mx / width) - 1;
		float Pny = -1 * ((2 * my / height) - 1);

		// Desproyectar
		Mat4 MPI;
		inverse(projMat, &MPI);
		Vec4 Rn = {Pnx, Pny, -1, 0};
		Vec4 Rv;
		matXvec(MPI, Rn, &Rv);


		// Omitir la cámara
		Vec4 Rm;
		Rv.z = -1;
		Rv.w = 0;
		Mat4 MVI;
		inverse(view, &MVI);
		matXvec(MVI, Rv, &Rm);
		Rm.w = 0;
		normalize(&Rm);
		//printf("(%f, %f, %f, %f\n)", Rm.x, Rm.y, Rm.z, Rm.w);
		shoot=1;
		//shootNewLaser(Rm.x*2.5,Rm.y*2.5,Rm.z*2);
		laserX=Rm.x*5;
		laserY=Rm.y*5;
		laserZ=Rm.z*4;


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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	mIdentity(&csMat);
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
	//asignar posicion a la luz de la nave
	shipLightX=shipX;
	shipLightY=shipY;
	shipLightZ=shipZ;


	if(shoot){
		shoot=0;
		shootNewLaser(cameraX+shipX,-cameraY+shipY,cameraZ+shipZ,laserX,laserY,laserZ);
	}

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

	numberDraw(number,n1->puntuacion,bgProgram,viewMatrixLocBG,projMatrixLocBG,modelMatrixLocBG);


	glUseProgram(programId);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, view.values);

	drawLaserBeams();






	glUseProgram(lightProgramId);
	//envio fuentes de luz
	lights[20] = cameraX+shipX;
	lights[21] = -cameraY+shipY;
	lights[22] = cameraZ+shipZ;
	lights[24]= -sin((angleZ*M_PI)/180.0);
	lights[25]= sin((angleY*M_PI)/180.0);
	lights[26]=-1.0;
	glBindBuffer(GL_UNIFORM_BUFFER, lightsBufferId);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lights), lights, GL_DYNAMIC_DRAW);
	GLuint uniformBlockIndex = glGetUniformBlockIndex(lightProgramId, "LightBlock");
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsBufferId);
	glUniformBlockBinding(lightProgramId, uniformBlockIndex, 0);

	glUniformMatrix4fv(projMatrixLocIl, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(viewMatrixLocIl, 1, GL_TRUE, view.values);

	glUniform3f(cameraPositionLoc, cameraX+shipX, -cameraY+shipY, cameraZ+shipZ);

	drawAsteroids();

	glUseProgram(programId);
	glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, projMat.values);
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, view.values);
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, shipMat.values);
	nave_draw(n1);
	int i=0;
	mIdentity(&shipMat);
	translate(&shipMat,1.1,0.9,0);
	scale(&shipMat,0.06,0.1,0.2);
	for(i=0;i<n1->hp;i++){
		translate(&shipMat,-3,0,0);
		glUseProgram(programId);
		glUniformMatrix4fv(projMatrixLoc, 1, GL_TRUE, identity.values);
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, identity.values);
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, shipMat.values);
		nave_draw(n1);
	}

	if(n1->hp<=0){
		gameOver=true;
		startGame=false;
	}

	glutSwapBuffers();
}

static void displayChoice(){
	Mat4 temp;
	if(gameOver){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mIdentity(&temp);
		glUseProgram(bgProgram);
		glUniformMatrix4fv(projMatrixLocBG, 1, GL_TRUE, projMat.values);
		glUniformMatrix4fv(modelMatrixLocBG, 1, GL_TRUE,temp.values);
		glUniformMatrix4fv(viewMatrixLocBG, 1, GL_TRUE, temp.values);
		BackgroundDraw(gameOverScreen);
		glutSwapBuffers();
	}
	if(startGame){
		display();
	}
	if(!startGame&&!gameOver){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mIdentity(&temp);
		glUseProgram(bgProgram);
		glUniformMatrix4fv(projMatrixLocBG, 1, GL_TRUE, projMat.values);
		glUniformMatrix4fv(modelMatrixLocBG, 1, GL_TRUE,temp.values);
		glUniformMatrix4fv(viewMatrixLocBG, 1, GL_TRUE, temp.values);
		BackgroundDraw(titleScreen);
		glutSwapBuffers();

	}
}

static void startMotionFunc(int key, int x, int y) {
	motion = key;


	if(key==GLUT_KEY_SHIFT_L||key=='A'){
		accionacc |=deacc;

		//accionacc &= 0XFD;
	}}

static void endMotionFunc(int key, int x, int y) {
	if(key==GLUT_KEY_SHIFT_L)
		accionacc &= 0XFD;
	motion=0;
	}


// =================================== //
// 				MAIN				   //
// =================================== //

int main(int argc, char **argv) {
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	//glutInitWindowSize(600, 600);
	//glutInitWindowPosition(100, 100);
	glutTimerFunc(50, timerFunc, 1);

	glutCreateWindow("Demo proyecto final");
	glutEnterGameMode();
	glutDisplayFunc(displayChoice);
	glutKeyboardFunc(startKey);
	glutKeyboardUpFunc(endKey);
	glutReshapeFunc(reshapeFunc);
	glutSpecialFunc(startMotionFunc);
	glutSpecialUpFunc(endMotionFunc);
	glutMouseFunc(mouseClick);
	glutSetCursor(GLUT_CURSOR_DESTROY);
	glewInit();
	initShaders();
	initLight();
	createShape();
	glutMainLoop();

	destroyAsteroids();
	nave_destroy(n1);
	destroyLaserBeams();
	free(asteroids);
	return 0;
}
