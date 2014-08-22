#ifndef ZCODE_h
#define ZCODE_h

#include "common.h"
#include "lil.h"

typedef struct {
    lil_t *pzlil; // only the parity mat
    int m;  // parity chunk size
    int k;  // data chunk size
    int datasize; // total datasize
    int packetsize; 
    int blocksize;
} z_info_t, *pz_info_t;

/**
 * init zcode information.
 */
int z_init(z_info_t *pzinfo, int m, int k, int datasize, int packetsize);

/**
 * free zcode information memory.
 */
int z_free(z_info_t *pzinfo);

/**
 * encode psrc data and store the parity data to pdes.
 * 
 * psrc, and pdes already alloc enough memory.
 */
int z_encode(z_info_t *pzinfo, unsigned char *psrc, unsigned char *pdes);
int z_encode_dpg(z_info_t *pzinfo, unsigned char *psrc, unsigned char *pdes);

/**
 * repair node data chunk, store data to pdes.
 *
 * data in psrc place the same sequence of phelp;
 * psrc, and pdes already alloc enough memory.
 */
// int z_repair(z_info_t *pzinfo, unsigned char *psrc, blist_t *phelp,int node, unsigned char *pdes);

#endif
