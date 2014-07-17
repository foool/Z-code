#ifndef MCODING_H
#define MCODING_H

#include "mat.h"

int get_block_size(int file_size, gfmat_t *pmat);

int mxcoding_pbg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, gfmat_t *pmat);
int mxcoding_ppg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, gfmat_t *pmat);

#endif
