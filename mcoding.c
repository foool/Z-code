/*
 *  mcoding.c
 *  
 *  Perform coding by calculating matrix product of the PGM and data blocks
 *  Adopt Jianqiang Luo's PPG(Parity Packet Guided) and PWG(Parity Wrod Guided) algorithm
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "mcoding.h"
#include "region_xor.h"

#define segment_size 256

int get_block_size(int file_size, int m, int k){
    int col;
    int block_size;

    col = LIL_COL(m, k);
               
    if( file_size%(segment_size*col) == 0 ){ 
        block_size = file_size/col;
    }else{
        block_size = (file_size/(col*segment_size)+1)*segment_size;
    }  

    return block_size;
}

int mxcoding_pbg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, lil_t *pzlil, int m, int k){
    int row, col;
    int i, j, p;
    int times;
    int rest_size;
    int pgfsize;
    gfele_t val;
    gfele_t *pgdes, *pgsrc;

    assert(block_size >= packet_size);

    row = m;
    col = LIL_COL(m,k);
    times = block_size/packet_size;
    pgfsize = packet_size/sizeof(gfele_t);

    rest_size = block_size - times*packet_size;

    memset(pdes, 0, row*block_size);

    if(rest_size == 0){
        for(i = 0; i < row; ++i){
            for(j = 0; j < col; ++j){
                val = (gfele_t)pzlil[i*col+j];
                pgsrc = (gfele_t *)(psrc + j*block_size);
                pgdes = (gfele_t *)(pdes + val*block_size);
                for(p = 0; p < times; ++p){
                    region_xor(pgdes, pgsrc, packet_size);
                    pgdes = pgdes + pgfsize;
                    pgsrc = pgsrc + pgfsize;
                }
            }
        }
    }else{
        printf("not multiple!\n");
        for(i = 0; i < row; ++i){
            for(j = 0; j < col; ++j){
                val = (gfele_t)pzlil[i*col+j];
                pgsrc = (gfele_t *)(psrc + j*block_size);
                pgdes = (gfele_t *)(pdes + val*block_size);
                printf("(%d, %d) = %d\n",i, j, val);
                for(p = 0; p < times; ++p){
                    region_xor(pgdes, pgsrc, packet_size);
                    pgdes = pgdes + pgfsize;
                    pgsrc = pgsrc + pgfsize;
                }
                region_xor(pgdes, pgsrc, rest_size);
            }
        }
    }
    return 1;
}


/*
int mxcoding_ppg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, lil_t *pzlil, int m, int k){
    int row, col;
    int i, j, p;
    int times;
    int rest_size;
    int bgfsize, pgfsize;
    gfele_t val;
    gfele_t *pgdes, *pgsrc;

    assert(block_size >= packet_size);

    row = m;
    col = LIL_COL(m, k);
    times = block_size/packet_size;
    printf("times = %d\n", times);
    bgfsize = block_size/sizeof(gfele_t);
    pgfsize = packet_size/sizeof(gfele_t);
    printf("block size = %d\n", block_size);

    memset(pdes, 0, row*block_size);

    printf("Encoding begin\n");
    rest_size = block_size - times*packet_size;
    if(rest_size == 0){
        printf("Encoding rest size == 0\n");
        pgdes = (gfele_t *)(pdes);
        for(i = 0; i < row; ++i){
            for(p = 0; p < times; ++p){
                pgsrc = (gfele_t *)(psrc + p*packet_size);
                for(j = 0; j < col; ++j){
                    val = (gfele_t )pzlil[j*m+i];
                    region_xor(pgdes, pgsrc+val*bgfsize, packet_size);
                }
                pgdes = pgdes + pgfsize;
            }
        }
    }else{
        printf("Encoding rest size != 0\n");
        pgdes = (gfele_t *)(pdes);
        for(i = 0; i < row; ++i){
            for(p = 0; p < times; ++p){
                printf("times : %d\n",p);
                pgsrc = (gfele_t *)(psrc + p*packet_size);
                for(j = 0; j < col; ++j){
                    val = (gfele_t )pzlil[j*m+i];
                    region_xor(pgdes, pgsrc+val*bgfsize, packet_size);
                }
                pgdes = pgdes + pgfsize;
            }
            pgsrc = (gfele_t *)(psrc + times*packet_size);
            for(j = 0; j < col; ++j){
                val = (gfele_t )pzlil[j*m+i];
                region_xor(pgdes, pgsrc+val*bgfsize, packet_size);
            }
            pgdes = pgdes + rest_size/sizeof(gfele_t);
        }
    }

    return 1;
}
*/
