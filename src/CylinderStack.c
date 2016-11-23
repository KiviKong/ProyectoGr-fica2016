
#include "CylinderStack.h"
#include <stdio.h>

CylinderStack Stack_create() {
	CylinderStack new = (CylinderStack) malloc(sizeof(CylinderStack));
    new->top = 0;
    return new;
};

void Stack_destroy(CylinderStack stack) {
	int i;
	for(i = 0; i < stack->top; i++) {
		cylinder_destroy(stack->stk[i]);
	}
	free(stack->stk);
	free(stack);
}

void push(CylinderStack s, Cylinder c) {
    if(s->top == (MAX - 1))
        s->top = 0;
    s->stk[s->top++] = c;
    printf("pushed new: MAX: %d\n", MAX);
};

Cylinder pop(CylinderStack s) {
    if(s->top == -1)
        return NULL;
	Cylinder popped = s->stk[s->top--];
	cylinder_destroy(s->stk[s->top--]);
    return popped;
};
