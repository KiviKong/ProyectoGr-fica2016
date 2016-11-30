/*
 * Numbers.h
 *
 *  Created on: 29/11/2016
 *      Author: Rodolfo
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include "Transforms.h"
#include "Mat4.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef SRC_NUMBERS_H_
#define SRC_NUMBERS_H_

struct strNumbers{
	float vertexPos[12];
	float vertexCol[12];
	GLuint indexArr[4];
	GLuint vertexId;
	GLuint* bufferId;
	GLuint texture[1];
	GLfloat textureArr1[8];
	GLfloat textureArr2[8];
	GLfloat textureArr3[8];
	GLfloat textureArr4[8];
	GLfloat textureArr5[8];
	GLfloat textureArr6[8];
};

typedef struct strNumbers* Numbers;

Numbers createNumbers();
void numberBind(Numbers b, GLuint vLoc, GLuint cLoc, GLuint tLoc);

void numberDraw(Numbers b, int puntuacion, GLuint programId,GLuint view,GLuint proj, GLuint model);


#endif /* SRC_NUMBERS_H_ */
