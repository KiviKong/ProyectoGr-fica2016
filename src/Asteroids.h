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
	GLuint *cylinderIndex;
	int vertexNum;
	int indexNum;
	GLuint vertexId;
	GLuint* indexBufferId;

};
typedef struct strAsteroid* Asteroid;
Asteroid Asteroid_create(float length, float bottomRadius, float topRadius,
		int slices, int stacks, float bottomColor[3], float topColor[3]);
void Asteroid_bind(Asteroid a, GLuint vLoc, GLuint cLoc);
void Asteroid_destroy(Asteroid);
void Asteroid_draw(Asteroid);


#endif /* ASTEROIDS_H_ */
