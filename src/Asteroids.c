/*
 * Asteroids.c
 *
 *  Created on: 15/10/2016
 *      Author: kevin
 */
#include <stdio.h>
#include "Asteroids.h"


Asteroid Asteroid_create(double r, int lats, int longs){
	Asteroid new = (Asteroid) malloc(sizeof(struct strAsteroid));
	new->vertexNum = ((lats * 2) + 2) * longs * 3;
	new->indexNum = (((lats * 2) + 2) * longs) * longs;
	new->vertexPos = (float*) malloc(new->vertexNum * sizeof(float));
	new->vertexCol = (float*) malloc(new->vertexNum * sizeof(float));
	new->AsteroidIndex = (GLuint*) malloc(new->indexNum * sizeof(GLuint));

	float colorR = 0.5;
	float colorG = 0.5;
	float colorB = 0.5;
	float colorChangeR = (0.7 - 0.5) / longs;
	float colorChangeG = (0.7 - 0.5) / longs;
	float colorChangeB = (0.7 - 0.5) / longs;


	int i, j;
	int pos =0, col =0;
	int ind = 0;
	int indv = 0;
	float lng, x, y, ruido,lat0,lat1,z0,z1,zr0,zr1;
	      for(i = 0; i < lats; i++) {
	    	  ruido = rand() % 15;
	    	  ruido /= 100;
	    	  ruido = 1 - ruido;

	           lat0 = M_PI * (-0.5 + (float) (i - 1) / lats);
	           z0  = sin(lat0) *r;
	           zr0 =  cos(lat0)*r;

	           lat1 = M_PI * (-0.5 + (float) i / lats);
	           z1 = sin(lat1) *r;
	           zr1 = cos(lat1) *r;


	           for(j = 0; j <= longs; j++) {
	        	   //if(j==longs) lng = 2 * M_PI * (float) (0 - 1) / longs;

	               lng = 2 * M_PI * (float) (j - 1) / longs;
	               x = cos(lng);
	               y = sin(lng);

	               new->vertexPos[pos++] = x * zr0;
	               new->vertexPos[pos++] = y * zr0;
	               new->vertexPos[pos++] = z0;
	               new->AsteroidIndex[ind++] = indv++;

	               new->vertexCol[col++] = (colorR ) * ruido;
	               new->vertexCol[col++] = (colorG ) * ruido;
	               new->vertexCol[col++] = (colorB ) * ruido;

	               new->vertexPos[pos++] = x * zr1;
	               new->vertexPos[pos++] = y * zr1;
	               new->vertexPos[pos++] = z1 ;
	               new->AsteroidIndex[ind++] = indv++;

	               new->vertexCol[col++] = (colorR+colorChangeR) * ruido;
	               new->vertexCol[col++] = (colorG+colorChangeG) * ruido;
	               new->vertexCol[col++] = (colorB+colorChangeB) * ruido;

	           }

	           new->AsteroidIndex[ind++] = 0xFFFF;


	           colorR += colorChangeR;
	           colorG += colorChangeG;
	           colorB += colorChangeB;

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
		a->AsteroidIndex,
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

