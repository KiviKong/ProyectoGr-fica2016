
#include <stdio.h>
#include "LaserBeam.h"
#include "Utils.h"

LaserBeam LaserBeam_create() {
    LaserBeam laser = (LaserBeam) malloc(sizeof(struct strLaserBeam));

    // Rectangle 1
    laser->vertexPos[0] = -0.05;
    laser->vertexPos[1] = 0;
    laser->vertexPos[2] = 4;

    laser->vertexPos[3] = -0.05;
    laser->vertexPos[4] = 0;
    laser->vertexPos[5] = 1;

    laser->vertexPos[6] = 0.05;
    laser->vertexPos[7] = 0;
    laser->vertexPos[8] = 4;

    laser->vertexPos[9] = 0.05;
    laser->vertexPos[10] = 0;
    laser->vertexPos[11] = 1;

    // Rectangle 2
    laser->vertexPos[12] = 0;
    laser->vertexPos[13] = 0.05;
    laser->vertexPos[14] = 4;

    laser->vertexPos[15] = 0;
    laser->vertexPos[16] = 0.05;
    laser->vertexPos[17] = 1;

    laser->vertexPos[18] = 0;
    laser->vertexPos[19] = -0.05;
    laser->vertexPos[20] = 4;

    laser->vertexPos[21] = 0;
    laser->vertexPos[22] = -0.05;
    laser->vertexPos[23] = 1;

    int i;
    for (i = 0; i < 9; i++) {
        laser->indexArr[i] = (i % 4 == 0 && i > 0) ? 0xFFFF : i;
        printf("%d\n", laser->indexArr[i]);
    }

    for(i = 0; i < 24; i++) {
        laser->vertexCol[i] = 1.0;
    }

    return laser;
};

void LaserBeam_bind(LaserBeam laser, GLuint vLoc, GLuint cLoc) {
    glGenVertexArrays(1, &laser->vertexId);
    laser->bufferId = (GLuint*) malloc(3 * sizeof(GLuint));
    glGenBuffers(3, laser->bufferId);

    // Vertex array
    glBindVertexArray(laser->vertexId);
    glBindBuffer(GL_ARRAY_BUFFER, laser->bufferId[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        (24 * sizeof(float)),
        laser->vertexPos,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(vLoc, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(vLoc);

    // Color array
    glBindBuffer(GL_ARRAY_BUFFER, laser->bufferId[1]);
    glBufferData(
        GL_ARRAY_BUFFER,
        (24 * sizeof(float)),
        laser->vertexCol,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(cLoc, 3, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(cLoc);

    // Element array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, laser->bufferId[2]);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        (9 * sizeof(GLuint)),
        laser->indexArr,
        GL_STATIC_DRAW
    );
};

void LaserBeam_draw(LaserBeam laser) {
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xFFFF);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(laser->vertexId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, laser->bufferId[2]);

    glDrawElements(
        GL_TRIANGLE_STRIP,
        9,
        GL_UNSIGNED_INT,
        0
    );
};

void LaserBeam_destroy(LaserBeam laser) {

};

void LaserBeam_update(LaserBeam laser) {

};
