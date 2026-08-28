#pragma once

typedef struct Matrix Matrix;

#define MAT_AT(mat, r, c) ((mat).data[(r) * (mat).col + (c)])

extern Matrix create_matrix(int, int);
extern void mat_rotate_2d(float*, int);
extern void mat_identity(float*);
extern void mat_rotate(float*, float, float, float, int);