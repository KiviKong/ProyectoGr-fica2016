/*
 * Hit.c
 *
 *  Created on: 30/11/2016
 *      Author: Rodolfo
 */

#include <stdio.h>
#include "Hit.h"
#include "Utils.h"



Hitstun HitCreate(){
	Hitstun bg = (Hitstun) malloc(sizeof(struct strHit));
	    bg->vertexPos[0] = -1;
	    bg->vertexPos[1] = 1;
	    bg->vertexPos[2] = -1;

	    bg->vertexPos[3] = -1;
	    bg->vertexPos[4] = -1;
	    bg->vertexPos[5] = -1;

	    bg->vertexPos[6] = 1;
	    bg->vertexPos[7] = 1;
	    bg->vertexPos[8] = -1;

	    bg->vertexPos[9] = 1;
	    bg->vertexPos[10] = -1;
	    bg->vertexPos[11] = -1;

	    int i = 0;

	    for(i = 0; i < 16; i+=4) {
	    	bg->vertexCol[i] = 1.0;
	        bg->vertexCol[i+2] = 0;
	        bg->vertexCol[i+3] = 0;
	        bg->vertexCol[i+4] = 0.3;
	    }
	    for (i = 0; i < 4; i++) {
	        bg->indexArr[i] = i;
	   }

	    return bg;
}
void HitBind(Hitstun n, GLuint vLoc, GLuint cLoc){
			glGenVertexArrays(1,&n->vertexId);
			n->bufferId = (GLuint*) malloc(3 * sizeof(GLuint));
			glGenBuffers(3, n->bufferId);

			glBindVertexArray(n->vertexId);
			glBindBuffer(GL_ARRAY_BUFFER, n->bufferId[0]);
			glBufferData(GL_ARRAY_BUFFER, (12) * sizeof(float), n->vertexPos,
			GL_STATIC_DRAW);
			glVertexAttribPointer(vLoc, 3, GL_FLOAT, 0, 0, 0);
			glEnableVertexAttribArray(vLoc);

			glBindBuffer(GL_ARRAY_BUFFER, n->bufferId[1]);
			glBufferData(GL_ARRAY_BUFFER, (16) * sizeof(float), n->vertexCol,
			GL_STATIC_DRAW);
			glVertexAttribPointer(cLoc, 4, GL_FLOAT, 0, 0, 0);
			glEnableVertexAttribArray(cLoc);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, n->bufferId[2]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (4) * sizeof(GLuint),
							n->indexArr,
							GL_STATIC_DRAW);

}
void HitDraw(Hitstun n){
			//glEnable(GL_CULL_FACE);
			//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(n->vertexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, n->bufferId[2]);

			//glDrawArrays(GL_TRIANGLE_STRIP,0,c->vertexNum*sizeof(float));
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
}
void HitDestroy(Hitstun n){
	free(n->bufferId);
	free(n);

}

void ResetAlpha(Hitstun h){
	h->vertexCol[3]=0.3;
	h->vertexCol[7]=0.3;
	h->vertexCol[11]=0.3;
	h->vertexCol[15]=0.3;
}

void MinusAlpha(Hitstun h){
	if(h->vertexCol[3]>0){
	h->vertexCol[3]-=0.03;
	h->vertexCol[7]-=0.3;
	h->vertexCol[11]-=0.3;
	h->vertexCol[15]-=0.3;
	}

}

