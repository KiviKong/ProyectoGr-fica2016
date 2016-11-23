/*
 * Cylinder.c
 *
 *  Created on: 06/10/2016
 *      Author: Rodolfo
 */
#include <stdio.h>
#include "Cylinder.h"


Cylinder cylinder_create(float length, float bottomRadius, float topRadius,
		int slices, int stacks, float bottomColor[3], float topColor[3], float coordX, float coordY, float coordZ) {
	Cylinder new = (Cylinder) malloc(sizeof(struct strCylinder));
	new->vertexNum = ((slices * 2) + 2) * stacks * 3;
	new->indexNum = (((slices * 2) + 2) * stacks) + stacks-1;
	new->vertexPos = (float*) malloc(new->vertexNum * sizeof(float));
	new->vertexCol = (float*) malloc(new->vertexNum * sizeof(float));
	new->cylinderIndex = (GLuint*) malloc(new->indexNum * sizeof(GLuint));
	new->coord[0] = coordX;
	new->coord[1] = coordY;
	new->coord[2] = coordZ;


	float lengthChange = length / stacks; //cambio de altura para cada stack
	float currentLength = length / 2; //punto y donde inicia primer stack
	float angle = (2 * M_PI) / slices;
	float radius = topRadius;
	float radiusChange = (bottomRadius - topRadius) / stacks;
	float currentAngle = 0;
	float colorR = topColor[0];
	float colorG = topColor[1];
	float colorB = topColor[2];
	float colorChangeR = (bottomColor[0] - topColor[0]) / stacks;
	float colorChangeG = (bottomColor[1] - topColor[1]) / stacks;
	float colorChangeB = (bottomColor[2] - topColor[2]) / stacks;
	int pos = 0;
	int col = 0; //indices para llenar arreglos posicion y color
	int ind = 0;
	int indv = 0;
	int i, j;
	for (i = 0; i < stacks; i++) {
		for (j = 0; j <=slices; j++) {
			if(j==slices)currentAngle=0;
			float ruido = rand() % 15;
			ruido /= 100;
			ruido = 1 - ruido;
			new->vertexPos[pos++] = cosf(currentAngle) * (radius);
			new->vertexPos[pos++] = -sinf(currentAngle) * (radius);
			new->vertexPos[pos++] = currentLength;

			new->vertexCol[col++] = (colorR ) * ruido;
			new->vertexCol[col++] = (colorG ) * ruido;
			new->vertexCol[col++] = (colorB ) * ruido;

			new->cylinderIndex[ind++] = indv++;

			new->vertexPos[pos++] = cosf(currentAngle) * (radius+radiusChange);
			new->vertexPos[pos++] = -sinf(currentAngle) * (radius+radiusChange);
			new->vertexPos[pos++] = currentLength-lengthChange;

			new->vertexCol[col++] = (colorR+colorChangeR) * ruido;
			new->vertexCol[col++] = (colorG+colorChangeG) * ruido;
			new->vertexCol[col++] = (colorB+colorChangeB) * ruido;

			new->cylinderIndex[ind++] = indv++;

			currentAngle += angle;

		}
		if(i!=stacks-1)
		new->cylinderIndex[ind++] = 0xFFFF;
		//new->cylinderIndex[ind++]=indv++;
		colorR += colorChangeR;
		colorG += colorChangeG;
		colorB += colorChangeB;
		currentAngle = 0;
		radius += radiusChange;
		currentLength -= lengthChange;
	}

	return new;
}

void cylinder_bind(Cylinder c, GLuint vLoc, GLuint cLoc) {

	glGenVertexArrays(1,&c->vertexId);
	c->indexBufferId = (GLuint*) malloc(3 * sizeof(GLuint));
	glGenBuffers(3, c->indexBufferId);

	glBindVertexArray(c->vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, c->indexBufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, (c->vertexNum) * sizeof(float), c->vertexPos,
	GL_STATIC_DRAW);
	glVertexAttribPointer(vLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vLoc);

	glBindBuffer(GL_ARRAY_BUFFER, c->indexBufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, (c->vertexNum) * sizeof(float), c->vertexCol,
	GL_STATIC_DRAW);
	glVertexAttribPointer(cLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(cLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->indexBufferId[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (c->indexNum) * sizeof(GLuint),
			c->cylinderIndex,
			GL_STATIC_DRAW);

}

void cylinder_destroy(Cylinder c) {
	free(c->vertexPos);
	free(c->vertexCol);
	free(c->cylinderIndex);
	free(c->indexBufferId);
	free(c);
}

void cylinder_draw(Cylinder c) {
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(c->vertexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->indexBufferId[2]);

	glDrawElements(GL_TRIANGLE_STRIP, c->indexNum, GL_UNSIGNED_INT, 0);
}

float cylinder_update(Cylinder c) {
	if (c->coord[2] == -2000)
		// do something
	return 0;
}
