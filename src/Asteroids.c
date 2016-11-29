/*
 * Asteroids.c
 *
 *  Created on: 15/10/2016
 *      Author: kevin
 */
#include <stdio.h>
#include "Asteroids.h"

#define rock 0


Asteroid create_asteroid2(float radius, int P, int M){
	Asteroid new = (Asteroid) malloc(sizeof(struct strAsteroid));
		new->vertexNum = (P + 1) * M*3;
		new->vertexColNum=(P + 1) * M*4;
		new->indexNum = 2 * P * M + 3 * P;
		new->vertexPos = (float*) malloc(new->vertexNum * sizeof(float));
		new->vertexNorm = (float*) malloc(new->vertexNum * sizeof(float));
		new->vertexCol = (float*) malloc(new->vertexNum * sizeof(float));
		new->AsteroidIndex = (GLuint*) malloc(new->indexNum * sizeof(GLuint));
		new->lats = P;
		new->longs = M;
		new->r = radius;
		new->hp=(rand()%2)+1;

		float colorR = 0.2;
		float colorG = 0.2;
		float colorB = 0.2;

		// float colorR = 0.6196;
		// float colorG = 0.4078;
		// float colorB = 0.0627;
		float random1;

	GLfloat phi = 0;
		GLfloat inc_phi = M_PI / P;
		GLfloat theta = 0;
		/*sphereVertexCount = (P + 1) * M;
		spherePos   = (GLfloat*) malloc(sizeof(GLfloat) * 3 * sphereVertexCount);
		sphereCol   = (GLfloat*) malloc(sizeof(GLfloat) * 3 * sphereVertexCount);
		sphereTex   = (GLfloat*) malloc(sizeof(GLfloat) * 2 * sphereVertexCount);*/

		setbuf(stdout, 0);
		float ruido;

	//	F�rmulas de Bessel
		GLfloat inc_theta = 2 * M_PI / M;
		int i = 0, p, m;
		for(p = 1; p <= P + 1; p ++) {
			theta = 0;
			for(m = 1; m <= M; m ++) {
				random1 = (((rand() % 2) + 9) / 10.0);
				ruido = rand() % 25;
						ruido /= 100.0;
						ruido = 1 - ruido;
				GLfloat x = sin(phi) * cos(theta);
				GLfloat y = sin(phi) * sin(theta);
				GLfloat z = cos(phi);

				new->vertexPos[i    ] = radius * x*random1;
				new->vertexPos[i + 1] = radius * y*random1;
				new->vertexPos[i + 2] = radius * z*random1;

				// new->vertexNorm[i    ] = new->vertexPos[i];
				// new->vertexNorm[i + 1] = new->vertexPos[i+1];
				// new->vertexNorm[i + 2] = new->vertexPos[i+2];

				new->vertexNorm[i    ] = new->vertexPos[i] * ruido;
				new->vertexNorm[i + 1] = new->vertexPos[i+1] * ruido;
				new->vertexNorm[i + 2] = new->vertexPos[i+2] * ruido;

				new->vertexCol[i    ] = (colorR) * ruido;
				new->vertexCol[i + 1] = (colorG) * ruido;
				new->vertexCol[i + 2] = (colorB) * ruido;


	//			Mapeo de coordenadas esf�ricas a plano
				/*sphereTex[j    ] = 0.5 + atan2(z, x) / (2 * M_PI);  // atan2(z, x) = arcotangente(z / x)
				sphereTex[j + 1] = 0.5 - asin(y) / M_PI;

				if(sphereTex[j]     > 0.5) sphereTex[j] = 1 - sphereTex[j];
				if(sphereTex[j + 1] > 0.5) sphereTex[j + 1] = 1 - sphereTex[j + 1];
				sphereTex[j]     *= 4;
				sphereTex[j + 1] *= 4;*/

				i += 3;
				//j += 2;
				theta += inc_theta;
			}
			phi += inc_phi;
		}




		GLushort v = 0, k = 0;
		for(p = 1; p <= P; p ++) {
			int firstV = v;
			for(m = 1; m <= M; m ++) {
				new->AsteroidIndex[k ++] = v;
				new->AsteroidIndex[k ++] = v + M;
				v ++;
			}
			new->AsteroidIndex[k ++] = firstV;
			new->AsteroidIndex[k ++] = firstV + M;
			new->AsteroidIndex[k ++] = 0xFFFF;
		}
		new->speed = 0;
			new->x = rand() % 180 + (-90);
			new->y = rand() % 180 + (-90);
			new->z = -(rand() % 1000)-1000;

		return new;
}

void Asteroid_bind(Asteroid a, GLuint vLoc, GLuint cLoc, GLuint vertexNormalLoc) {
	glGenVertexArrays(1, &a->vertexId);
	a->indexBufferId = (GLuint*) malloc(4 * sizeof(GLuint));
	glGenBuffers(4, a->indexBufferId);
	glBindVertexArray(a->vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, a->indexBufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, (a->vertexNum) * sizeof(float), a->vertexPos,
	GL_STATIC_DRAW);
	glVertexAttribPointer(vLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vLoc);

	glBindBuffer(GL_ARRAY_BUFFER, a->indexBufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, (a->vertexNum) * sizeof(float), a->vertexCol,
	GL_STATIC_DRAW);
	glVertexAttribPointer(cLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(cLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a->indexBufferId[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (a->indexNum) * sizeof(GLuint),
			a->AsteroidIndex,
			GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, a->indexBufferId[3]);
	glBufferData(GL_ARRAY_BUFFER, (a->vertexNum)*sizeof(float), a->vertexNorm, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexNormalLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vertexNormalLoc);

}
void Asteroid_destroy(Asteroid a) {
	glDeleteVertexArrays(1,a->AsteroidIndex);
	glDeleteBuffers(1,a->indexBufferId);
	free(a->vertexPos);
	free(a->vertexCol);
	free(a->AsteroidIndex);
	free(a->indexBufferId);
	free(a);
}
void Asteroid_draw(Asteroid a) {
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(a->vertexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a->indexBufferId[2]);

	glDrawElements(GL_TRIANGLE_STRIP, a->indexNum, GL_UNSIGNED_INT, 0);

}

float updateAsteroidZ(Asteroid a,float velocity) {


	if (a->z >= 0){
		a->x = rand() % 180 + (-90);
		a->y = rand() % 180 + (-90);
		return a->z = -2000;

	}
	else
		return a->z += (a->speed*velocity);

}

void setVelAsteroid(Asteroid a, float vel) {
	a->speed = vel;
}

bool Asteroid_collide(Asteroid a, Cylinder c) {
	float astMinX = a->x - a->r;
	float astMaxX = a->x + a->r;

	float cylMinX = c->coord[0] - 0.1;
	float cylMaxX = c->coord[0] + 0.1+c->velX;

	float astMinY = a->y - a->r;
	float astMaxY = a->y + a->r;

	float cylMinY = c->coord[1] - 0.1;
	float cylMaxY = c->coord[1] + 0.1+c->velY;

	// if ((asteroids[iterator]->z + asteroids[iterator]->speed < (cameraZ + 1 - 7))
	// 		&& (asteroids[iterator]->z + asteroids[iterator]->speed > (cameraZ - 1 - asteroids[iterator]->speed - 7))) {
	if((astMaxX>cylMinX && astMaxX<cylMaxX)||(astMinX>cylMinX && astMinX<cylMaxX)){
		if((astMaxY>cylMinY&& astMaxY<cylMaxY)||(astMinY>cylMinY&& astMinY<cylMaxY)){
			return true;
		}
	}else if((cylMaxX>astMinX && cylMaxX<astMaxX)||(cylMinX>astMinX && cylMinX<astMaxX)){
		if((cylMaxY>astMinY && cylMaxY<astMaxY)||(cylMinY>astMinY && cylMinY<astMaxY)){
			return true;
		}
	}

	return false;
};
void resetAsteroidZ(Asteroid new){
	new->z = -(rand() % 1000)-1000;
}
