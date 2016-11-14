#ifndef BACKGROUND_H_
#define BACKGROUND_H_ value

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct strBg {
    float vertexPos[12];
    float vertexCol[12];
    GLuint indexArr[4];
    GLuint vertexId;
    GLuint* bufferId;
};

typedef struct strBg* Background;

Background BackgroundCreate(float minX, float maxX, float minY, float maxY, float depth);
void BackgroundBind(Background b, GLuint vLoc, GLuint cLoc);
void BackgroundDraw(Background b);
void BackgroundDestroy(Background b);

#endif
