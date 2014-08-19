/*
 *  lil.c
 *  
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lil.h"

static int lil_pgmk2m2[8] = {
    0,3,
    1,2,
    0,2,
    1,3
};

static int lil_pgmk2m3[18] = {
    0,4,8,
    1,5,6,
    2,3,7,
    0,3,6,
    1,4,7,
    2,5,8
};

static int lil_pgmk2m4[32] = {
    0,5,10,15,
    1,6,11,12,
    2,7,8,13,
    3,4,9,14,
    0,4,8,12,
    1,5,9,13,
    2,6,10,14,
    3,7,11,15
};

static int lil_init_k2(lil_t *lilk2, int m){
    switch(m){
        case 2:
            memcpy(lilk2, &lil_pgmk2m2[0], 8*sizeof(lil_t));
            break;
        case 3:
            memcpy(lilk2, &lil_pgmk2m3[0], 18*sizeof(lil_t));
            break;
        case 4:
            memcpy(lilk2, &lil_pgmk2m4[0], 32*sizeof(lil_t));
            break;
        default:
            printf("m only must be 2,3,4\n");
            return 0;
    }

    return 1;
}

static 
inline
int region_mul_plus(lil_t *plil, int mul, int add, int len){
    int i;

    for(i = 0; i < len; ++i){
        *(plil+i) = (*(plil+i))*mul+add;
    }

    return len;
}

static int lil_ins_k(lil_t *lil_new, lil_t *lil_ori, int m, int k){
    int r;
    int add;
    int i,j;
    int row;
    lil_t * psrc = lil_ori;
    lil_t * pdes = lil_new;

    r = LIL_R(m, k);

    /* first k nodes */ 
    for(i = 0; i < k; ++i){
        for(j = 0; j < r; ++j){
            for(add = 0; add < m; add++){
                memcpy(pdes, psrc, m*sizeof(lil_t));
                region_mul_plus(pdes, m, add, m);
                pdes = pdes+m;
            }
            psrc = psrc+m;
        }
    }

    /* 
     * last node 
     */
    memcpy(pdes, pdes-r*m*m, r*m*m*sizeof(lil_t));
    for(i = 0; i < r*m; ++i){
        for(row = 0; row < m; row++, pdes++){
            (*pdes) = ((*pdes)/m)*m+(((*pdes)%m)+row)%m;
        }
    }

    return 1;
}


int make_pgm_lil(lil_t *pzlil, int m, int k){
    lil_t *lt1, *lt2;
    int ki;

    lt1 = NULL;
    lt2 = NULL;

    if((m > 4)||(m < 2)){
        printf("Error: m must be one of 2/3/4\n");
        return 0;
    }
    
    ki = 2;
    lt1 = (lil_t *)LIL_ALLOC(m, 2);
    lil_init_k2(lt1, m);

    if(k > 2){
        for(ki = 3; ki <= k; ++ki){
            lt2 = (lil_t *)LIL_ALLOC(m, ki);
            lil_ins_k(lt2, lt1, m, ki-1);
            free(lt1);
            lt1 = (lil_t *)LIL_ALLOC(m, ki);
            memcpy(lt1, lt2, m*LIL_COL(m, ki)*sizeof(lil_t));
        }
        memcpy(pzlil, lt2, LIL_COL(m,k)*m*sizeof(lil_t));
    }else{
        memcpy(pzlil, lt1, LIL_COL(m,k)*m*sizeof(lil_t));
    }

    lil_free(lt1);
    if(lt2 != NULL){
        lil_free(lt2);
    }

    printf("====\n");
    print_matrix(pzlil, LIL_ROW(m,k), LIL_COL(m,k));
    printf("====\n");
    return 1;
    
}

void lil_free(lil_t *pzlil){
    free(pzlil);
}

void print_flat(lil_t *pzlil, int row, int col){
    int i;

    for(i = 0; i < row*col; ++i){
        if((i%row == 0)&&(i != 0))  printf("\t");
        printf("%2d ", *(pzlil+i));
    }

}

void print_matrix(lil_t *pzlil, int row, int col){
    int i, j; 

    for(i = 0; i < row; i++){
        for(j = 0; j < col; ++j){
            printf("%2d ", *(pzlil+row*j+i));
        }
        printf("\n");
    }
}
