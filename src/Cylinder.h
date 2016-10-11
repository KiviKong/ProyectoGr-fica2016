/*
 * Cylinder.h
 *
 *  Created on: 06/10/2016
 *      Author: Rodolfo
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>


#ifndef CYLINDER_H_
#define CYLINDER_H_

struct strCylinder {
	float (*vertexPos);
	float *vertexCol;
	GLuint *cylinderIndex;
	int vertexNum;
	int indexNum;
	GLuint vertexId;
	GLuint* indexBufferId;

};
typedef struct strCylinder* Cylinder;
Cylinder cylinder_create(float length, float bottomRadius,
float topRadius, int slices,
int stacks,
float bottomColor[3], float topColor[3]);
void cylinder_bind(Cylinder c, GLuint vLoc, GLuint cLoc);
void cylinder_destroy(Cylinder);
void cylinder_draw(Cylinder);
void printarray(Cylinder);


#endif /* CYLINDER_H_ */
