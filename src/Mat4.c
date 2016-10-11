
#include <stdio.h>
#include "Mat4.h"

void mIdentity(Mat4 *m) {
	int i;
	for(i = 0; i < 16; i ++) {
		m->values[i] = i % 5 == 0? 1 : 0;
	}
}

void mMult(Mat4 *m1, Mat4 m2) {
	int c, r, k;
	Mat4 m3;
	for(c = 0; c < 4; c ++) {
		for(r = 0; r < 4; r ++) {
			float sum = 0;
			for(k = 0; k < 4; k ++) {
				sum += m1->at[c][k] * m2.at[k][r];
			}
			m3.at[c][r] = sum;
		}
	}
	for(c = 0; c < 16; c ++) m1->values[c] = m3.values[c];
}

void mPrint(Mat4 m) {
	int r, c;
	printf(" ---------------------\n");
	for(r = 0; r < 4; r ++) {
		printf("| ");
		for(c = 0; c < 4; c ++) {
			printf("%.2f ", m.at[r][c]);
		}
		printf("|\n");
	}
	printf(" ---------------------\n");
}

