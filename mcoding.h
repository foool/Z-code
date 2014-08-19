#ifndef MCODING_H
#define MCODING_H

#include "lil.h"

int get_block_size(int file_size, int m, int k);

int mxcoding_pbg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, lil_t *pzlil, int m, int k);
//int mxcoding_ppg(unsigned char *pdes, unsigned char *psrc, int block_size, int packet_size, lil_t *pzlil, int m, int k);

#endif
