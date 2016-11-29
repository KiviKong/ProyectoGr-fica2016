/*
 * Asteroids.h
 *
 *  Created on: 15/10/2016
 *      Author: kevin
 */


#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Cylinder.h"

struct strAsteroid {
	float (*vertexPos);
	float *vertexCol;
	float *vertexNorm;
	GLuint *AsteroidIndex;
	int vertexNum;
	int indexNum;
	int vertexColNum;
	GLuint vertexId;
	GLuint* indexBufferId;
	float x, y, z, speed;
	int lats;
	int longs;
	float r;
	int hp;

};
typedef struct strAsteroid* Asteroid;
Asteroid Asteroid_create(float r, int lats, int longs, int feo);
Asteroid create_asteroid2(float radius, int P, int M);
void Asteroid_bind(Asteroid a, GLuint vLoc, GLuint cLoc, GLuint vertexNormalLoc);
void Asteroid_destroy(Asteroid);
void Asteroid_draw(Asteroid);
float updateAsteroidZ(Asteroid a,float velocity);
void setVelAsteroid(Asteroid a, float vel);
bool Asteroid_collide(Asteroid a, Cylinder c);
void resetAsteroidZ(Asteroid new);


#endif /* ASTEROIDS_H_ */
