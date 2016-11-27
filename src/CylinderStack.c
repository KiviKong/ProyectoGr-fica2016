
#include "CylinderStack.h"
#include <stdio.h>

CylinderStack Stack_create() {
	CylinderStack new = (CylinderStack) malloc(sizeof(CylinderStack));
    new->top = 0;
    new->stk=(Cylinder*)malloc(sizeof(Cylinder)*MAX);
    int i;
    for(i=0;i<MAX;i++){
    	new->stk[i]=NULL;
    }
    return new;
};

void Stack_destroy(CylinderStack stack) {
	int i;
	for(i = 0; i < stack->top; i++) {
		if(stack->stk[i]!=NULL)
		cylinder_destroy(stack->stk[i]);
	}
	free(stack->stk);
	free(stack);
}

void push(CylinderStack s, Cylinder c) {
	int i=0;
    for(;i<MAX;i++){
    	if(s->stk[i]==NULL){
    		s->stk[i] = c;
    		s->top++;
    		break;
    	}
    }

};

Cylinder pop(CylinderStack s) {
    if(s->top == -1)
        return NULL;
	Cylinder popped = s->stk[s->top--];
	cylinder_destroy(s->stk[s->top]);
    return popped;
};
