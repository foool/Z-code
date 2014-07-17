#ifndef MAT_H
#define MAT_H

#include <stdint.h>

#define mat_init(pmat)  \
        do{             \
            (pmat)->data = NULL;  \
            (pmat)->row  = 0;     \
            (pmat)->col  = 0;     \
        }while(0)

#define sizeofmat(row, col) ((row)*(col)*sizeof(gfele_t))

#define BRC_BLOCK(m,k) (math_pow((m),(k-1)))
#define BRC_ROW(m,k) ((m)*(math_pow((m),(k-1))))
#define BRC_COL(m,k) ((k)*(math_pow((m),(k-1))))
#define BRC_CALLOC(m,k) (calloc(1,(BRC_ROW((m),(k))*BRC_COL((m),(k))*sizeof(int))))

typedef uint32_t gfele_t;

typedef struct{
    int row;
    int col;
    gfele_t * data;
} gfmat_t, *pgfmat_t;

inline int math_pow(int exp, int index);
int make_parity_brc_bitmatrix(gfmat_t *pmat, int n, int k);

int mat_free(gfmat_t *mat);

#endif
