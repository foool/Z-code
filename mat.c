/*
 *  mat.c
 *  
 *  matrix  manipulations for generating PGM of Z codes
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mat.h"

static int gmk2m2[16] = {\
    1, 0, 1, 0, 
    0, 1, 0, 1, 
    0, 1, 1, 0, 
    1, 0, 0, 1\
};

static int gmk2m3[54] = {\
    1, 0, 0, 1, 0, 0, 
    0, 1, 0, 0, 1, 0, 
    0, 0, 1, 0, 0, 1, 
    0, 0, 1, 1, 0, 0, 
    1, 0, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 1, 
    0, 1, 0, 1, 0, 0, 
    0, 0, 1, 0, 1, 0, 
    1, 0, 0, 0, 0, 1\
};

static int gmk2m4[128] = {\
    1, 0, 0, 0, 1, 0, 0, 0, 
    0, 1, 0, 0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 0, 1, 0, 
    0, 0, 0, 1, 0, 0, 0, 1, 
    0, 0, 0, 1, 1, 0, 0, 0, 
    1, 0, 0, 0, 0, 1, 0, 0, 
    0, 1, 0, 0, 0, 0, 1, 0, 
    0, 0, 1, 0, 0, 0, 0, 1, 
    0, 0, 1, 0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 1, 0, 0, 
    1, 0, 0, 0, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 0, 0, 1, 
    0, 1, 0, 0, 1, 0, 0, 0, 
    0, 0, 1, 0, 0, 1, 0, 0, 
    0, 0, 0, 1, 0, 0, 1, 0, 
    1, 0, 0, 0, 0, 0, 0, 1\
};

static int init_k2(int *pm, int m){
    switch(m){
        case 2:
            memcpy(pm, &gmk2m2[0], 16*sizeof(int));
            break;
        case 3:
            memcpy(pm, &gmk2m3[0], 54*sizeof(int));
            break;
        case 4:
            memcpy(pm, &gmk2m4[0], 128*sizeof(int));
            break;
        default:
            printf("m only equals to 2,3,4\n");
            return 0;
    }

    return 1;
}

static int ins_k(int *pm_new, int *pm_ori, int m, int k){
    int row_ori, col_ori;
    int col_new;
    int i, j, p;
    int inc;
    int pos;

    row_ori = BRC_ROW(m, k);
    col_ori = BRC_COL(m, k);
    col_new = BRC_COL(m, k+1);
    
    for(i = 0; i < row_ori; ++i){
        for(j = 0; j < col_ori; ++j){
            if(pm_ori[i*col_ori+j] == 0) continue;
            //else, pm_ori(i,j) = 1
            pos = (i*m)*col_new+(j*m);
            for(p = 0; p < m; ++p){
                inc = p*col_new+p;
                pm_new[pos+inc] = 1;
            }
        }
    }
    
    //last column
    for(i = 0; i < row_ori; ++i){
        int shift = i/BRC_BLOCK(m,k);
        for(j = (k-1)*BRC_BLOCK(m,k); j < col_ori; ++j){
            if(pm_ori[i*col_ori+j] == 0) continue;
            pos = (i*m)*col_new+(j*m+BRC_BLOCK(m,k+1));
            for(p = 0; p < m; ++p){
                if(shift+p < m){
                    inc = p*col_new+p+shift*col_new;
                }else{
                    inc = p*col_new+p+(shift-m)*col_new;
                }
                pm_new[pos+inc] = 1;
            }
        }
    }
    return 1;
}

static
inline
int resize_matrix(gfmat_t * pmat, int row, int col)
{
    
    assert(pmat != NULL);

    if((row == 0)||(col == 0)){
        pmat->row = 0;
        pmat->col = 0;
        pmat->data = NULL;
        
        return 0;
    }

    if(pmat->data == NULL){
        pmat->data = (gfele_t *)malloc(sizeofmat(row, col));
    }else{
        if((pmat->row)*(pmat->col) != row*col){
            pmat->data = (gfele_t *)realloc(pmat->data, sizeofmat(row, col));
        }
    }

    pmat->row = row;
    pmat->col = col;

    return row;
}

inline
int math_pow(int exp, int index){
    int i, accum;
    for(i = 0, accum=1; i < index; ++i){
        accum = accum*exp;
    }

    return accum;
}

int make_parity_brc_bitmatrix(gfmat_t *pmat, int n, int k)
{
    int m, ki;
    int *pm1, *pm2;

    assert(sizeof(gfele_t) == sizeof(int));

    m = n-k;
    pm1 = NULL;
    pm2 = NULL;

    if((m > 4)||(m < 2)){
        printf("Error: m must be one of 2/3/4\n");
        return 0;
    }
    
    resize_matrix(pmat, BRC_ROW(m, k), BRC_COL(m, k));

    ki = 2;
    pm1 = (int *)BRC_CALLOC(m, k);
    init_k2(pm1, m);

    if(k > 2){
        for(ki = 3; ki <= k; ++ki){
            pm2 = (int *)BRC_CALLOC(m, ki);
            ins_k(pm2, pm1, m, ki-1);
            free(pm1);
            pm1 = (int *)BRC_CALLOC(m, ki);
            memcpy(pm1, pm2, BRC_ROW(m, ki)*BRC_COL(m, ki)*sizeof(int));
        }
        memcpy(pmat->data, (gfele_t *)pm2, BRC_ROW(m, k)*BRC_COL(m, k)*sizeof(int));
    }else{
        memcpy(pmat->data, (gfele_t *)pm1, BRC_ROW(m, k)*BRC_COL(m, k)*sizeof(int));
    }
    free(pm1);
    if(pm2 != NULL){
        free(pm2);
    }

    return 1;
}

int mat_free(gfmat_t *mat){
    if(mat->data != NULL){
        free(mat->data);
        mat->data = NULL;
    }
    mat->row = 0;
    mat->col = 0;

    return 1;
}
