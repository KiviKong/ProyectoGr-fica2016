
#include <stdio.h>
#include "Background.h"

Background BackgroundCreate(float minX, float maxX, float minY, float maxY, float depth) {
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
    int i = 0;
    for(i = 0; i < 12; i++) {
        bg->vertexCol[i] = 0.5;
    }
    for (i = 0; i < 4; i++) {
        bg->indexArr[i] = i;
    }
    return bg;
};

void BackgroundBind(Background b, GLuint vLoc, GLuint cLoc) {
    glGenVertexArrays(1, &b->vertexId);
    b->bufferId = (GLuint*) malloc(3 * sizeof(GLuint));
    glGenBuffers(3, b->bufferId);

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

};
void BackgroundDraw(Background b){
    glBindVertexArray(b->vertexId);
    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        b->bufferId[2]
    );

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
