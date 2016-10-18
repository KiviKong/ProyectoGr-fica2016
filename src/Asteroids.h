/*
 * Asteroids.h
 *
 *  Created on: 15/10/2016
 *      Author: kevin
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

struct strAsteroid {
	float (*vertexPos);
	float *vertexCol;
	GLuint *AsteroidIndex;
	int vertexNum;
	int indexNum;
	GLuint vertexId;
	GLuint* indexBufferId;
	int x, y, z, speed;

};
typedef struct strAsteroid* Asteroid;
Asteroid Asteroid_create(double r, int lats, int longs, int feo);
void Asteroid_bind(Asteroid a, GLuint vLoc, GLuint cLoc);
void Asteroid_destroy(Asteroid);
void Asteroid_draw(Asteroid);
float updateAsteroidZ(Asteroid a);
void setVelAsteroid(Asteroid a, float vel);


#endif /* ASTEROIDS_H_ */
