#ifndef LASER_BEAM_H_
#define LASER_BEAM_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct strLaserBeam {
    GLfloat vertexPos[24];
    GLfloat vertexCol[24];
    GLuint indexArr[9];
    GLuint vertexId;
    GLuint* bufferId;
    float coordX;
    float coordY;
};

typedef struct strLaserBeam* LaserBeam;

LaserBeam LaserBeam_create();
void LaserBeam_bind(LaserBeam laser, GLuint vLoc, GLuint cLoc);
void LaserBeam_draw(LaserBeam laser);
void LaserBeam_destroy(LaserBeam laser);
void LaserBeam_update(LaserBeam laser);


#endif
