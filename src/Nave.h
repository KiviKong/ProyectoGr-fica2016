/*
 * nave.h
 *
 *  Created on: 11/10/2016
 *      Author: Rodolfo
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>


#ifndef NAVE_H_
#define NAVE_H_

struct strNave {
	GLuint vertexId;
	GLuint* indexBufferId;

};
typedef struct strNave* Nave;
Nave nave_create();
void nave_bind(Nave n, GLuint vLoc, GLuint cLoc);
void nave_destroy(Nave);
void nave_draw(Nave);
void printarraynave(Nave);


#endif /* NAVE_H_ */
