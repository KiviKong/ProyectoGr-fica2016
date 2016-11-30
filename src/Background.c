
#include <stdio.h>
#include "Background.h"
#include "Utils.h"

Background BackgroundCreate(float minX, float maxX, float minY, float maxY, float depth,int type) {
    Background bg = (Background) malloc(sizeof(struct strBg));
    bg->vertexPos[0] = minX;
    bg->vertexPos[1] = maxY;
    bg->vertexPos[2] = depth;

    bg->vertexPos[3] = minX;
    bg->vertexPos[4] = minY;
    bg->vertexPos[5] = depth;

    bg->vertexPos[6] = maxX;
    bg->vertexPos[7] = maxY;
    bg->vertexPos[8] = depth;

    bg->vertexPos[9] = maxX;
    bg->vertexPos[10] = minY;
    bg->vertexPos[11] = depth;
    if(type==0){
    bg->textureArr[0] = 0;  // v
    bg->textureArr[1] = 5;  // u

    bg->textureArr[2] = 0;  // u
    bg->textureArr[3] = 0;  // v

    bg->textureArr[4] = 5;  // u
    bg->textureArr[5] = 5;  // v

    bg->textureArr[6] = 5;  // v
    bg->textureArr[7] = 0;  // u
    }else if(type==1){
    	bg->textureArr[0] = -2;  // v
    	bg->textureArr[1] = 1.5;  // u

        bg->textureArr[2] = -2;  // u
    	bg->textureArr[3] = -2.25;  // v

   	    bg->textureArr[4] = 1;  // u
   	    bg->textureArr[5] = 1.5;  // v

   	    bg->textureArr[6] = 1;  // v
   	    bg->textureArr[7] = -2.25;  // u
    }

    int i = 0;
    for(i = 0; i < 12; i++) {
        bg->vertexCol[i] = 0.9;
    }
    for (i = 0; i < 4; i++) {
        bg->indexArr[i] = i;
    }
    return bg;
};

void BackgroundBind(Background b, GLuint vLoc, GLuint cLoc, GLuint tLoc,int texture) {
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
        b->textureArr,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(tLoc, 2, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(tLoc);

	glGenTextures(1, b->texture);
	if(texture==0)
		loadTexture("textures/space-bg.bmp", b->texture[0]);
	else if(texture==1)
		loadTexture("textures/titlescreen.bmp", b->texture[0]);
	else if(texture==2){
		printf("texture loaded");
		loadTexture("textures/gameover.bmp", b->texture[0]);
		printf("texture loaded");
	}
};


void BackgroundDraw(Background b){
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
};

void BackgroundDestroy(Background b) {
    free(b->vertexPos);
    free(b->vertexCol);
    free(b->indexArr);
    free(&b->vertexId);
    free(b->bufferId);
    free(b);
};
