/*
 * Asteroids.c
 *
 *  Created on: 15/10/2016
 *      Author: kevin
 */
#include <stdio.h>
#include "Asteroids.h"

#define rock 0

Asteroid Asteroid_create(float r, int lats, int longs, int feo) {
	Asteroid new = (Asteroid) malloc(sizeof(struct strAsteroid));
	new->vertexNum = ((lats * 2) + 2) * longs * 3;
	new->indexNum = (((lats * 2) + 2) * longs) * longs;
	new->vertexPos = (float*) malloc(new->vertexNum * sizeof(float));
	new->vertexCol = (float*) malloc(new->vertexNum * sizeof(float));
	new->AsteroidIndex = (GLuint*) malloc(new->indexNum * sizeof(GLuint));
	new->lats = lats;
	new->longs = longs;
	new->r = r;

	float colorR = 0.6196;
	float colorG = 0.4078;
	float colorB = 0.0627;
	float colorChangeR = (0.7 - 0.6) / longs;
	float colorChangeG = (0.45 - 0.35) / longs;
	float colorChangeB = (0.1 - 0.04) / longs;

	int i, j;
	int pos = 0, col = 0;
	int ind = 0;
	int indv = 0;
	float lng, x, y, ruido, lat0, lat1, z0, z1, zr0, zr1;
	float random0, random1;
	for (i = 0; i < lats; i++) {
		ruido = rand() % 5;
		ruido /= 100.0;
		ruido = 1 - ruido;
		//ruido=1;

		lat0 = M_PI * (-0.5 + (float) (i - 1) / lats);
		z0 = (sin(lat0) * r)/**(((rand()%2)+9)/10)*/;
		zr0 = (cos(lat0) * r)/**(((rand()%2)+9)/10)*/;

		lat1 = M_PI * (-0.5 + (float) i / lats);
		z1 = (sin(lat1) * r)/**(((rand()%2)+9)/10)*/;
		zr1 = (cos(lat1) * r)/**(((rand()%2)+9)/10)*/;
		if (i == 0)
			random0 = (((rand() % 2) + 9) / 10.0);
		else
			random0 = random1;
		random1 = (((rand() % 2) + 9) / 10.0);

		for (j = 0; j <= longs; j++) {
			//if(j==longs) lng = 2 * M_PI * (float) (0 - 1) / longs;

			lng = 2 * M_PI * (float) (j - 1) / longs;
			x = cos(lng) * (((rand() % 2) + 9) / 10.0);
			y = sin(lng) * (((rand() % 2) + 9) / 10.0);

			//if (i == 0) {
				new->vertexPos[pos++] = (x * zr0) * random0;
				new->vertexPos[pos++] = (y * zr0) * random0;
				new->vertexPos[pos++] = (z0) * random0;
			/*} else {
				new->vertexPos[pos++] = new->vertexPos[pos-(longs*6)+2];
				new->vertexPos[pos++] = new->vertexPos[pos-(longs*6)+2];
				new->vertexPos[pos++] = new->vertexPos[pos-(longs*6)+2];
			}*/

			/*if(feo ==0)
			 new->AsteroidIndex[ind++] = rand() % new->vertexNum;
			 else*/
			new->AsteroidIndex[ind++] = indv++;

			new->vertexCol[col++] = (colorR) * ruido;
			new->vertexCol[col++] = (colorG) * ruido;
			new->vertexCol[col++] = (colorB) * ruido;

			new->vertexPos[pos++] = (x * zr1) * random1;
			new->vertexPos[pos++] = (y * zr1) * random1;
			new->vertexPos[pos++] = (z1) * random1;

			/*if(feo ==0)
			 new->AsteroidIndex[ind++] = rand() % new->vertexNum;
			 else*/
			new->AsteroidIndex[ind++] = indv++;

			new->vertexCol[col++] = (colorR + colorChangeR) * ruido;
			new->vertexCol[col++] = (colorG + colorChangeG) * ruido;
			new->vertexCol[col++] = (colorB + colorChangeB) * ruido;

		}

		new->AsteroidIndex[ind++] = 0xFFFF;

		/*colorR += colorChangeR;
		colorG += colorChangeG;
		colorB += colorChangeB;*/

	}
	new->speed = 0;
	new->x = rand() % 100 + (-50);
	new->y = rand() % 100 + (-50);
	new->z = 0;
	return new;

}

Asteroid create_asteroid2(float radius, int P, int M){
	Asteroid new = (Asteroid) malloc(sizeof(struct strAsteroid));
		new->vertexNum = (P + 1) * M*3;
		new->indexNum = 2 * P * M + 3 * P;
		new->vertexPos = (float*) malloc(new->vertexNum * sizeof(float));
		new->vertexNorm = (float*) malloc(new->vertexNum * sizeof(float));
		new->vertexCol = (float*) malloc(new->vertexNum * sizeof(float));
		new->AsteroidIndex = (GLuint*) malloc(new->indexNum * sizeof(GLuint));
		new->lats = P;
		new->longs = M;
		new->r = radius;

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
		int i = 0, j = 0, p, m;
		for(p = 1; p <= P + 1; p ++) {
			theta = 0;
			for(m = 1; m <= M; m ++) {
				random1 = (((rand() % 2) + 9) / 10.0);
				ruido = rand() % 5;
						ruido /= 100.0;
						ruido = 1 - ruido;
				GLfloat x = sin(phi) * cos(theta);
				GLfloat y = sin(phi) * sin(theta);
				GLfloat z = cos(phi);

				new->vertexPos[i    ] = radius * x*random1;
				new->vertexPos[i + 1] = radius * y*random1;
				new->vertexPos[i + 2] = radius * z*random1;

				new->vertexNorm[i    ] = new->vertexPos[i];
				new->vertexNorm[i + 1] = new->vertexPos[i+1];
				new->vertexNorm[i + 2] = new->vertexPos[i+2];

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


	if (a->z >= 0)
		return a->z = -2000;
	else
		return a->z += (a->speed*velocity);

}

void setVelAsteroid(Asteroid a, float vel) {
	a->speed = vel;
}
