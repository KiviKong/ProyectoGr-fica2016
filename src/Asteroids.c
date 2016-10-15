/*
 * Asteroids.c
 *
 *  Created on: 15/10/2016
 *      Author: kevin
 */
#include <stdio.h>
#include "Asteroids.h"


Asteroid Asteroid_create(float length, float bottomRadius, float topRadius,
		int slices, int stacks, float bottomColor[3], float topColor[3]){
		Asteroid new = (Asteroid) malloc(sizeof(struct strAsteroid));
		new->vertexNum = ((slices * 2) + 2) * stacks * 3;
		new->indexNum = (((slices * 2) + 2) * stacks) + stacks-1;
		new->vertexPos = (float*) malloc(new->vertexNum * sizeof(float));
		new->vertexCol = (float*) malloc(new->vertexNum * sizeof(float));
		new->cylinderIndex = (GLuint*) malloc(new->indexNum * sizeof(GLuint));
		float lengthChange = length / stacks; //cambio de altura para cada stack
		float currentLength = length / 2; //punto y donde inicia primer stack
		float angle = (2 * M_PI) / slices;
		float radius = topRadius;
		float radiusChange = (bottomRadius *1.5 ) / (stacks/2);
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
		float cosa =0;
		for (i = 0; i < stacks; i++) {
			for (j = 0; j <=slices; j++) {
				if(j==slices)currentAngle=0;
				float ruido = rand() % 15;
				ruido /= 100;
				ruido = 1 - ruido;
				new->vertexPos[pos++] = cosf(currentAngle)
						* (radius);
				new->vertexPos[pos++] = currentLength;
				new->vertexPos[pos++] = -sinf(currentAngle)
						* (radius);

				new->vertexCol[col++] = (colorR ) * ruido;
				new->vertexCol[col++] = (colorG ) * ruido;
				new->vertexCol[col++] = (colorB ) * ruido;

				new->cylinderIndex[ind++] = indv++;

				if(i<stacks/2)
					cosa = radius + radiusChange;
				else
					cosa = radius - radiusChange;

				new->vertexPos[pos++] = cosf(currentAngle) * (cosa);
				new->vertexPos[pos++] = currentLength-lengthChange;
				new->vertexPos[pos++] = -sinf(currentAngle) * (cosa);

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
			if(i<stacks/2)
			radius += radiusChange;
			else
			radius -= radiusChange;
			currentLength -= lengthChange;
		}

		return new;

}
void Asteroid_bind(Asteroid a, GLuint vLoc, GLuint cLoc){
	glGenVertexArrays(1,&a->vertexId);
	a->indexBufferId = (GLuint*) malloc(3 * sizeof(GLuint));
	glGenBuffers(3, a->indexBufferId);
	glBindVertexArray(a->vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, a->indexBufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, (a->vertexNum) * sizeof(float), a->vertexPos,
	GL_STATIC_DRAW);
	glVertexAttribPointer(vLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vLoc);

	glBindBuffer(GL_ARRAY_BUFFER, a->indexBufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, (a->vertexNum) * sizeof(float), a->vertexCol,
	GL_STATIC_DRAW);
	glVertexAttribPointer(cLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(cLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a->indexBufferId[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (a->indexNum) * sizeof(GLuint),
		a->cylinderIndex,
		GL_STATIC_DRAW);

}
void Asteroid_destroy(Asteroid a){

}
void Asteroid_draw(Asteroid a){
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(a->vertexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a->indexBufferId[2]);


	glDrawElements(GL_TRIANGLE_STRIP, a->indexNum, GL_UNSIGNED_INT, 0);

}


