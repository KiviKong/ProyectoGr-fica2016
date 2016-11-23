
#include "CylinderStack.h"
#include <stdio.h>

CylinderStack Stack_create() {
	CylinderStack new = (CylinderStack) malloc(sizeof(CylinderStack));
    new->top = 0;
    return new;
};

void push(CylinderStack s, Cylinder c) {
    if(s->top == (MAX - 1))
        s->top = 0;
    s->stk[s->top++] = c;
    printf("pushed new: MAX: %d\n", MAX);
};

Cylinder pop(CylinderStack s, Cylinder c) {
    if(s->top == -1)
        return NULL;
    return s->stk[s->top--];
};
