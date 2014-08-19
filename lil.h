#ifndef MAT_H
#define MAT_H

#include <stdint.h>
#include "common.h"

#define LIL_R(m,k) (math_pow((m),(k-1)))
#define LIL_ROW(m,k) (m)
#define LIL_COL(m,k) ((k)*(math_pow((m),(k-1))))
#define LIL_ALLOC(m,k) ((lil_t *)malloc((m)*LIL_COL((m),(k))*sizeof(lil_t)))


typedef uint32_t gfele_t;
typedef int lil_t;

int make_pgm_lil(lil_t *pzlil, int m, int k);

void lil_free(lil_t *pzlil);

void print_flat(lil_t *pzlil, int row, int col);
void print_matrix(lil_t *pzlil, int row, int col);
#endif
