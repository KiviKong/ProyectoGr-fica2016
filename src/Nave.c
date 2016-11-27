/*
 * nave.c
 *
 *  Created on: 11/10/2016
 *      Author: Rodolfo
 */

#include "Nave.h"

int vertexNum=(10 + (10 + 4) + ((10 + 4) * 2) /*+ ((10 + 4) * 2)*/)*3;
int indexNum=10 + (10 + 4) + ((10 + 4) * 2) /*+ ((10 + 4) * 2)*/+6;
float vertexPos[156]={//parte frontal, parte trasera, alas, motores
			0,0,-1,	//frontal1
			-0.5,0,0, //frontal2
			0,0,-1, //frontal3
			0,0.5,0, //frontal4
			0,0,-1,//frontal5
			0.5,0,0,//frontal6
			0,0,-1,//frontal7
			0,-0.5,0,//frontal8
			0,0,-1,//frontal9
			-0.5,0,0,//frontal10
						//reinicio1
			-0.5,0,0,//trasera1
			-0.3,0,.3,//trasera2
			0,0.5,0,//trasera3
			0,0.3,0.3,//trasera4
			0.5,0,0,//trasera5
			0.3,0,0.3,//trasera6
			0,-0.5,0,//trasera7
			0,-0.3,0.3,//trasera8
			-0.5,0,0,//trasera9
			-0.3,0,0.3,//trasera10
						//reinicio2
			-0.3,0,0.3,//trasera11
			0,-0.3,0.3,//trasera12
			0,0.3,0.3,//trasera13
			0.3,0,0.3,//trasera14
						//reinicio3
			-0.5,0,0,//alaA1
			-0.8,0,0.3,//alaA2
			-0.5,0,0,//alaA3
			-1,0.8,0,//alaA4
			-0.5,0,0,//alaA5
			-0.8,0,-0.3,//alaA6
			-0.5,0,0,//alaA7
			-1,-.8,0,//alaA8
			-0.5,0,0,//alaA9
			-0.8,0,0.3,//alaA10
					//reinicio4
			-1,-.8,0,//alaA12
			-0.8,0,0.3,//alaA12
			-.8,0,-.3,//alaA13
			-1,0.8,0,//alaA14
					//reinicio5
			0.5,0,0,//alaB1
			0.8,0,0.3,//alaB2
			0.5,0,0,//alaB3
			1.0,.8,0,//alaB4
			0.5,0,0,//alaB5
			0.8,0,-0.3,//alaB6
			0.5,0,0,//alaB7
			1,-.8,0,//alaB8
			0.5,0,0,//alaB9
			0.8,0,0.3,//alaB10
					//reinicio6
			1,-.8,0,//alaB12
			0.8,0,0.3,//alaB12
			.8,0,-.3,//alaB13
			1,.8,0//alaB14
					//reinicio7



	};
	float vertexCol[156]={
			0,0,1,	//frontal1
						0.5,0,0, //frontal2
						0,0,1, //frontal3
						0,0.5,0, //frontal4
						0,0,1,//frontal5
						0.5,0,0,//frontal6
						0,0,1,//frontal7
						0,0.5,0,//frontal8
						0,0,1,//frontal9
						0.5,0,0,//frontal10
									//reinicio1
						0.5,0,0,//trasera1
						0.3,0,.3,//trasera2
						0,0.5,0,//trasera3
						0,0.3,0.3,//trasera4
						0.5,0,0,//trasera5
						0.3,0,0.3,//trasera6
						0,0.5,0,//trasera7
						0,0.3,0.3,//trasera8
						0.5,0,0,//trasera9
						0.3,0,0.3,//trasera10
									//reinicio2
						0.3,0,0.3,//trasera11
						0,0.3,0.3,//trasera12
						0.3,0,0.3,//trasera13
						0,0.3,0.3,//trasera14
									//reinicio3
						0.5,0,0,//alaA1
						0.8,0,0.3,//alaA2
						0.5,0,0,//alaA3
						1,0.8,0,//alaA4
						0.5,0,0,//alaA5
						0.8,0,0.3,//alaA6
						0.5,0,0,//alaA7
						1,.8,0,//alaA8
						0.5,0,0,//alaA9
						0.8,0,0.3,//alaA10
								//reinicio4
						0.8,0,0.3,//alaA11
						1,.8,0,//alaA12
						.8,0,.3,//alaA13
						1,0.8,0,//alaA14
								//reinicio5
						0.5,0,0,//alaB1
						0.8,0,0.3,//alaB2
						0.5,0,0,//alaB3
						1.0,.8,0,//alaB4
						0.5,0,0,//alaB5
						0.8,0,0.3,//alaB6
						0.5,0,0,//alaB7
						1,.8,0,//alaB8
						0.5,0,0,//alaB9
						0.8,0,0.3,//alaB10
								//reinicio6
						0.8,0,0.3,//alaB11
						1,.8,0,//alaB12
						.8,0,.3,//alaB13
						1,.8,0//alaB14
								//reinicio7
	};
	GLuint naveIndex[58]={
			0,1,2,3,4,5,6,7,8,9,0xFFFF,//parte frontal
			10,11,12,13,14,15,16,17,18,19,0xFFFF,//trasera1
			20,21,22,23,0xFFFF,//trasera2
			24,25,26,27,28,29,30,31,32,33,0xFFFF,//alaA1
			34,35,36,37,0xFFFF,//alaA2
			38,39,40,41,42,43,44,45,46,47,0xFFFF,//alaB1
			48,49,50,51//alaB2
	};
Nave nave_create() {
	Nave new = (Nave) malloc(sizeof(struct strNave));
	return new;
}

void nave_bind(Nave n,GLuint vLoc, GLuint cLoc){
	glGenVertexArrays(1,&n->vertexId);
		n->indexBufferId = (GLuint*) malloc(3 * sizeof(GLuint));
		glGenBuffers(3, n->indexBufferId);

		glBindVertexArray(n->vertexId);
		glBindBuffer(GL_ARRAY_BUFFER, n->indexBufferId[0]);
		glBufferData(GL_ARRAY_BUFFER, (vertexNum) * sizeof(float), vertexPos,
		GL_STATIC_DRAW);
		glVertexAttribPointer(vLoc, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(vLoc);

		glBindBuffer(GL_ARRAY_BUFFER, n->indexBufferId[1]);
		glBufferData(GL_ARRAY_BUFFER, (vertexNum) * sizeof(float), vertexCol,
		GL_STATIC_DRAW);
		glVertexAttribPointer(cLoc, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(cLoc);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, n->indexBufferId[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexNum) * sizeof(GLuint),
				naveIndex,
				GL_STATIC_DRAW);
}

void nave_destroy(Nave n){
		free(n->indexBufferId);
		free(n);
}
void nave_draw(Nave n){
	glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CW);
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(n->vertexId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, n->indexBufferId[2]);

		//glDrawArrays(GL_TRIANGLE_STRIP,0,c->vertexNum*sizeof(float));
		glDrawElements(GL_TRIANGLE_STRIP, indexNum, GL_UNSIGNED_INT, 0);
}
void printarraynave(Nave n){

}
short checkCollision(float navemaxX,float naveminX,float navemaxY,float naveminY, float objectmaxX,float objectminX, float objectmaxY,float objectminY){
	if((objectmaxX>naveminX && objectmaxX<navemaxX)||(objectminX>naveminX && objectminX<navemaxX)){
		if((objectmaxY>naveminY&& objectmaxY<navemaxY)||(objectminY>naveminY&& objectminY<navemaxY)){
			return 1;
		}
	}else if((navemaxX>objectminX && navemaxX<objectmaxX)||(naveminX>objectminX && naveminX<objectmaxX)){
		if((navemaxY>objectminY && navemaxY<objectmaxY)||(naveminY>objectminY && naveminY<objectmaxY)){
			return 1;
		}
	}
	return 0;
}
