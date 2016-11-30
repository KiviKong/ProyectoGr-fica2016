/*
 * Numbers.c
 *
 *  Created on: 29/11/2016
 *      Author: Rodolfo
 */

#include <stdio.h>
#include "Numbers.h"
#include "Utils.h"


Numbers createNumbers() {
	Numbers number = (Numbers) malloc(sizeof(struct strNumbers));
	number->vertexPos[0] = -0.3;
	number->vertexPos[1] = 0.5;
	number->vertexPos[2] = -1;

	number->vertexPos[3] = -0.3;
	number->vertexPos[4] = -0.5;
	number->vertexPos[5] = -1;

	number->vertexPos[6] = 0.3;
	number->vertexPos[7] = 0.5;
	number->vertexPos[8] = -1;

	number->vertexPos[9] = 0.3;
	number->vertexPos[10] = -0.5;
	number->vertexPos[11] = -1;

	number->textureArr1[0] = 0;  // v
	number->textureArr1[1] = 5;  // u

	number->textureArr1[2] = 0;  // u
	number->textureArr1[3] = 0;  // v

	number->textureArr1[4] = 5;  // u
	number->textureArr1[5] = 5;  // v

	number->textureArr1[6] = 5;  // v
	number->textureArr1[7] = 0;  // u

	number->textureArr2[0] = 0.1;  // v
	number->textureArr2[1] = 0.6;  // u

	number->textureArr2[2] = 0.2;  // u
	number->textureArr2[3] = 0.6;  // v

	number->textureArr2[4] = 0.1;  // u
	number->textureArr2[5] = 0.7;  // v

	number->textureArr2[6] = 0.2;  // v
	number->textureArr2[7] = 0.7;  // u

	number->textureArr3[0] = 0.1;  // v
	number->textureArr3[1] = 0.6;  // u

	number->textureArr3[2] = 0.2;  // u
	number->textureArr3[3] = 0.6;  // v

	number->textureArr3[4] = 0.1;  // u
	number->textureArr3[5] = 0.7;  // v

	number->textureArr3[6] = 0.2;  // v
	number->textureArr3[7] = 0.7;  // u

	number->textureArr4[0] = 0.1;  // v
	number->textureArr4[1] = 0.6;  // u

	number->textureArr4[2] = 0.2;  // u
	number->textureArr4[3] = 0.6;  // v

	number->textureArr4[4] = 0.1;  // u
	number->textureArr4[5] = 0.7;  // v

	number->textureArr4[6] = 0.2;  // v
	number->textureArr4[7] = 0.7;  // u

	number->textureArr5[0] = 0.1;  // v
	number->textureArr5[1] = 0.6;  // u

	number->textureArr5[2] = 0.2;  // u
	number->textureArr5[3] = 0.6;  // v

	number->textureArr5[4] = 0.1;  // u
	number->textureArr5[5] = 0.7;  // v

	number->textureArr5[6] = 0.2;  // v
	number->textureArr5[7] = 0.7;  // u

	number->textureArr6[0] = 0.1;  // v
	number->textureArr6[1] = 0.6;  // u

	number->textureArr6[2] = 0.2;  // u
	number->textureArr6[3] = 0.6;  // v

	number->textureArr6[4] = 0.1;  // u
	number->textureArr6[5] = 0.7;  // v

	number->textureArr6[6] = 0.2;  // v
	number->textureArr6[7] = 0.7;  // u

	int i = 0;
	for (i = 0; i < 12; i++) {
		number->vertexCol[i] = 1;
	}
	for (i = 0; i < 4; i++) {
		number->indexArr[i] = i;
	}
	return number;

}

void numberBind(Numbers b, GLuint vLoc, GLuint cLoc, GLuint tLoc) {
	glGenVertexArrays(1, &b->vertexId);
	    b->bufferId = (GLuint*) malloc(4 * sizeof(GLuint));
	    glGenBuffers(4, b->bufferId);

	    glBindVertexArray(b->vertexId);
	    glBindBuffer(GL_ARRAY_BUFFER, b->bufferId[0]);
	    glBufferData(
	        GL_ARRAY_BUFFER,
	        (12 * sizeof(float)),
	        b->vertexPos,
	        GL_STATIC_DRAW
	    );
	    glVertexAttribPointer(vLoc, 3, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(vLoc);

	    glBindBuffer(GL_ARRAY_BUFFER, b->bufferId[1]);
	    glBufferData(
	        GL_ARRAY_BUFFER,
	        (12 * sizeof(float)),
	        b->vertexCol,
	        GL_STATIC_DRAW
	    );
	    glVertexAttribPointer(cLoc, 3, GL_FLOAT, 0, 0, 0);
	    glEnableVertexAttribArray(cLoc);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->bufferId[2]);
	    glBufferData(
	        GL_ELEMENT_ARRAY_BUFFER,
	        (4 * sizeof(GLuint)),
	        b->indexArr,
	        GL_STATIC_DRAW
	    );

	    glBindBuffer(GL_ARRAY_BUFFER, b->bufferId[3]);
		glBufferData(
	        GL_ARRAY_BUFFER,
	        sizeof(GLfloat) * 8,
	        b->textureArr1,
	        GL_STATIC_DRAW
	    );
	    glVertexAttribPointer(tLoc, 2, GL_FLOAT, 0, 0, 0);
	    glEnableVertexAttribArray(tLoc);

	glGenTextures(1, b->texture);
	loadTexture("textures/text.bmp", b->texture[0]);

};

void numberDraw(Numbers b, int puntuacion, GLuint programId,GLuint view,GLuint proj, GLuint model){
	Mat4 identity;
	Mat4 csMat;
	mIdentity(&identity);
	mIdentity(&csMat);

	glUseProgram(programId);

	translate(&csMat,-1.1,-0.9,-15);
	scale(&csMat,50,50,50);
	translate(&csMat,3,0,0);

	glUniformMatrix4fv(proj, 1, GL_TRUE, identity.values);
	glUniformMatrix4fv(view, 1, GL_TRUE, identity.values);
	glUniformMatrix4fv(model, 1, GL_TRUE, csMat.values);


    glBindVertexArray(b->vertexId);

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        b->bufferId[2]
    );

    glBindTexture(GL_TEXTURE_2D, b->texture[0]);
    glDrawElements(
        GL_TRIANGLE_STRIP,
        4,
        GL_UNSIGNED_INT,
        0
    );


    translate(&csMat,3,0,0);

    glUniformMatrix4fv(model, 1, GL_TRUE, csMat.values);

    glBindTexture(GL_TEXTURE_2D, b->texture[0]);
    glDrawElements(
        GL_TRIANGLE_STRIP,
        4,
        GL_UNSIGNED_INT,
        0
    );


    translate(&csMat,3,0,0);

    glUniformMatrix4fv(model, 1, GL_TRUE, csMat.values);




    glBindTexture(GL_TEXTURE_2D, b->texture[0]);
    glDrawElements(
        GL_TRIANGLE_STRIP,
        4,
        GL_UNSIGNED_INT,
        0
    );


    translate(&csMat,3,0,0);

    glUniformMatrix4fv(model, 1, GL_TRUE, csMat.values);



    glBindTexture(GL_TEXTURE_2D, b->texture[0]);
    glDrawElements(
        GL_TRIANGLE_STRIP,
        4,
        GL_UNSIGNED_INT,
        0
    );


    translate(&csMat,3,0,0);

    glUniformMatrix4fv(model, 1, GL_TRUE, csMat.values);




    glBindTexture(GL_TEXTURE_2D, b->texture[0]);
    glDrawElements(
        GL_TRIANGLE_STRIP,
        4,
        GL_UNSIGNED_INT,
        0
    );


    translate(&csMat,3,0,0);

    glUniformMatrix4fv(model, 1, GL_TRUE, csMat.values);





    glBindTexture(GL_TEXTURE_2D, b->texture[0]);
    glDrawElements(
        GL_TRIANGLE_STRIP,
        4,
        GL_UNSIGNED_INT,
        0
    );

};

