/*
 *  mcoding.c
 *  
 *  Perform coding by calculating matrix product of the PGM and data blocks
 *  Adopt Jianqiang Luo's PPG(Parity Packet Guided) and PWG(Parity Wrod Guided) algorithm
 *
 */

#include <assert.h>
#include <string.h>

#include "mcoding.h"
#include "region_xor.h"

#define segment_size 256

int get_block_size(int file_size, gfmat_t *pmat)
{
    int col;
    int block_size;

    col = pmat->col;
               
    if( file_size%(segment_size*col) == 0 ){ 
        block_size = file_size/col;
    }else{
        block_size = (file_size/(col*segment_size)+1)*segment_size;
    }  

    return block_size;
}

int mxcoding_pbg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, gfmat_t *pmat)
{
    int row, col;
    int i, j, k;
    int times;
    int rest_size;
    int pgfsize;
    gfele_t val;
    gfele_t *pgdes, *pgsrc;

    assert(block_size >= packet_size);

    row = pmat->row;
    col = pmat->col;
    times = block_size/packet_size;
    pgfsize = packet_size/sizeof(gfele_t);

    rest_size = block_size - times*packet_size;

    memset(pdes, 0, row*block_size);

    if(rest_size == 0){
        for(i = 0; i < row; ++i){
            for(j = 0; j < col; ++j){
                val = pmat->data[i*col+j];
                if(val == 0) continue;
                pgsrc = (gfele_t *)(psrc + j*block_size);
                pgdes = (gfele_t *)(pdes + i*block_size);
                for(k = 0; k < times; ++k){
                    region_xor(pgdes, pgsrc, packet_size);
                    pgdes = pgdes + pgfsize;
                    pgsrc = pgsrc + pgfsize;
                }
            }
        }
    }else{
        for(i = 0; i < row; ++i){
            for(j = 0; j < col; ++j){
                val = pmat->data[i*col+j];
                if(val == 0) continue;
                pgsrc = (gfele_t *)(psrc + j*block_size);
                pgdes = (gfele_t *)(pdes + i*block_size);
                for(k = 0; k < times; ++k){
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

int mxcoding_ppg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, gfmat_t *pmat){
    int row, col;
    int i, j, k;
    int times;
    int rest_size;
    int bgfsize, pgfsize;
    gfele_t val;
    gfele_t *pgdes, *pgsrc;

    assert(block_size >= packet_size);

    row = pmat->row;
    col = pmat->col;
    times = block_size/packet_size;
    bgfsize = block_size/sizeof(gfele_t);
    pgfsize = packet_size/sizeof(gfele_t);

    memset(pdes, 0, row*block_size);

    rest_size = block_size - times*packet_size;
    if(rest_size == 0){
        pgdes = (gfele_t *)(pdes);
        for(i = 0; i < row; ++i){
            for(k = 0; k < times; ++k){
                pgsrc = (gfele_t *)(psrc + k*packet_size);
                for(j = 0; j < col; ++j){
                    val = pmat->data[i*col+j];
                    if(val == 1){
                        region_xor(pgdes, pgsrc, packet_size);
                    }
                    pgsrc = pgsrc + bgfsize;
                }
                pgdes = pgdes + pgfsize;
            }
        }
    }else{
        pgdes = (gfele_t *)(pdes);
        for(i = 0; i < row; ++i){
            for(k = 0; k < times; ++k){
                pgsrc = (gfele_t *)(psrc + k*packet_size);
                for(j = 0; j < col; ++j){
                    val = pmat->data[i*col+j];
                    if(val == 1){
                        region_xor(pgdes, pgsrc, packet_size);
                    }
                    pgsrc = pgsrc + bgfsize;
                }
                pgdes = pgdes + pgfsize;
            }
            pgsrc = (gfele_t *)(psrc + times*packet_size);
            for(j = 0; j < col; ++j){
                val = pmat->data[i*col+j];
                if(val == 1){
                    region_xor(pgdes, pgsrc, rest_size);
                }
                pgsrc = pgsrc + bgfsize;
            }
            pgdes = pgdes + rest_size/sizeof(gfele_t);
        }
    }

    return 1;
}
