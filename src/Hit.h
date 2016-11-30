/*
 * Hit.h
 *
 *  Created on: 30/11/2016
 *      Author: Rodolfo
 */

#ifndef SRC_HIT_H_
#define SRC_HIT_H_


#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct strHit {
    float vertexPos[12];
    float vertexCol[16];
    GLuint vertexId;
    GLuint* bufferId;
    GLuint indexArr[4];
};

typedef struct strHit* Hitstun;

Hitstun HitCreate();
void HitBind(Hitstun , GLuint , GLuint);
void HitDraw(Hitstun);
void HitDestroy(Hitstun);
void ResetAlpha(Hitstun);
void MinusAlpha(Hitstun h);


#endif /* SRC_HIT_H_ */
