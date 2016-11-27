#ifndef CYLINDER_STACK_H_
#define CYLINDER_STACK_H_

#include "Cylinder.h"

#define MAX 25

struct strStack {
    Cylinder* stk;
    int top;
};

typedef struct strStack* CylinderStack;

CylinderStack Stack_create();
void Stack_destroy(CylinderStack s);
void push(CylinderStack s, Cylinder c);
Cylinder pop(CylinderStack s);

#endif
