/*
 * Transforms.h
 *
 *  Created on: 07/09/2016
 *      Author: Rodolfo
 */

#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#include "Mat4.h"

void translate(Mat4*,float,float,float);
void rotateX(Mat4*,float);
void rotateY(Mat4*,float);
void rotateZ(Mat4*,float);
void scale(Mat4*,float,float,float);

void loadIdentity(Mat4* toIdentity);
void pushMatrix(Mat4* m);
int popMatrix(Mat4* popped);

void setOrtho(Mat4* projMatrix, float left, float right, float bottom,
		float top, float near, float far);
void setPerspective(Mat4* m,float fovy,float aspect, float nearz, float farz);

#endif /* TRANSFORMS_H_ */
