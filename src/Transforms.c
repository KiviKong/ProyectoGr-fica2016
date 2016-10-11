/*
 * Transforms.c
 *
 *  Created on: 07/09/2016
 *      Author: Rodolfo
 */

#include "Transforms.h"
#include <stdlib.h>
#include <math.h>

//#define toRadians (deg) deg * M_PI/180.0

struct strNode {
	Mat4* matrix;
	struct strNode* prior;
};

typedef struct strNode* Node;

Node top = NULL;

void loadIdentity(Mat4* toIdentity) {
	mIdentity(toIdentity);
	while (top!=NULL) {
		Node temp = top;
		top = top->prior;
		free(temp->matrix);
		free(temp);
	}

}
void pushMatrix(Mat4* m) {
	Mat4* mclon = (Mat4*) malloc(sizeof(Mat4));
	int i;
	for (i = 0; i < 16; i++) {
		mclon->values[i] = m->values[i];
	}
	Node new = (Node) malloc(sizeof(struct strNode));
	new->matrix = mclon;
	new->prior = top;
	top = new;

}
int popMatrix(Mat4* popped) {
	int i;
	if (top == NULL)
		return -1;
	Mat4* mTop = top->matrix;
	for (i = 0; i < 16; i++) {
		popped->values[i] = mTop->values[i];
	}
	Node temp = top;
	top = top->prior;
	free(temp->matrix);
	free(temp);
	return 0;
}

void translate(Mat4 *m, float tx, float ty, float tz) {
	//Construir matriz de trslacion tm
	Mat4 tm;
	mIdentity(&tm);
	tm.at[0][3] = tx;
	tm.at[1][3] = ty;
	tm.at[2][3] = tz;
	//multiplicar mt por la matriz recibida
	mMult(m, tm);
}
void rotateX(Mat4 *m, float angle) {
	//Construir matriz de rotacion xm
	float rad = angle * M_PI / 180.0;
	Mat4 xm;
	mIdentity(&xm);
	xm.at[1][1] = cosf(rad);
	xm.at[1][2] = -sinf(rad);
	xm.at[2][1] = sinf(rad);
	xm.at[2][2] = cosf(rad);

	mMult(m, xm);
}
void rotateY(Mat4 *m, float angle) {
	//Construir matriz de rotacion xm
	float rad = angle * M_PI / 180.0;
	Mat4 ym;
	mIdentity(&ym);
	ym.at[0][0] = cosf(rad);
	ym.at[0][2] = sinf(rad);
	ym.at[2][0] = -sinf(rad);
	ym.at[2][2] = cosf(rad);

	mMult(m, ym);
}
void rotateZ(Mat4 *m, float angle) {
	//Construir matriz de rotacion xm
	float rad = angle * M_PI / 180.0;
	Mat4 zm;

	mIdentity(&zm);
	zm.at[0][0] = cosf(rad);
	zm.at[0][1] = -sinf(rad);
	zm.at[1][0] = sinf(rad);
	zm.at[1][1] = cosf(rad);

	mMult(m, zm);
}
void scale(Mat4 *m, float sx, float sy, float sz) {
	Mat4 sm;
	mIdentity(&sm);
	sm.at[0][0] = sx;
	sm.at[1][1] = sy;
	sm.at[2][2] = sz;

	mMult(m, sm);
}

void setOrtho(Mat4* projMatrix, float left, float right, float bottom,
		float top, float near, float far){
	mIdentity(projMatrix);
	projMatrix->at[0][0]=2/(right-left);
	projMatrix->at[1][1]=2/(top-bottom);
	projMatrix->at[2][2]=2/(near-far);
	projMatrix->at[0][3]=-(right+left)/(right-left);
	projMatrix->at[1][3]=-(top+bottom)/(top-bottom);
	projMatrix->at[2][3]=-(near+far)/(near-far);


}

void setPerspective(Mat4* m,float fovy,float aspect, float nearz, float farz){
	mIdentity(m);
	float angle=fovy * M_PI / 180.0;
	m->at[0][0]=1/(aspect*tanf(angle/2));
	m->at[1][1]=1/tanf(angle/2);
	m->at[2][2]=(farz+nearz)/(nearz-farz);
	m->at[2][3]=(-2*nearz*farz)/(nearz-farz);
	m->at[3][3]=0;
	m->at[3][2]=-1;


}

