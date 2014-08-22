/*
 *  zcode.c
 *
 *  Interfaces of initialization, encoding, repairing and deconstruction for Z codes
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "zcode.h"
#include "common.h"
#include "mcoding.h"
#include "region_xor.h"

/*
static int find(int *list, int start, int end, int key)
{
    int i;
    for (i = start; i < end; i++) {
        if (list[i] == key) {
            return i;
        }
    }
    return -1;
}

static void init_list(blist_t *list, int len)
{
    list->len = len;
    if (len == 0) {
        list->plist = NULL;
    } else {
        list->plist = (int *)zalloc(sizeof(int)*len);
    }
}

static void free_list(blist_t *list)
{
    free(list->plist);
    list->plist = NULL;
}

// set k num of array to list
static void set_list(blist_t *list, int k, int *array)
{
    init_list(list, k);
    int i;
    for (i = 0; i < k; i++) {
        list->plist[i] = array[i];
    }
    list->len = i;
}

static int get_repair_list(blist_t *list, z_info_t *pzinfo, int node)
{
    int fragment = (pzinfo->pmat->col) / (pzinfo->k); //nums in one node
    int total = (pzinfo->k + pzinfo->m -1)*fragment/(pzinfo->m);
    int row[fragment]; // rows num of parity chunk
    int col[total - fragment]; // col num of data chunk
    int r_start, r_end;
    int c_start, c_end;
    int start = node * fragment, end = (node+1)*fragment-1;
    r_start = r_end = c_start = c_end = 0;
    int row_num, col_num;
    int pos=0;

    gfmat_t *pmat = pzinfo->pmat;

    // from the k-1 unit(apart from node) get the first col num 
    col_num = end +1;
    if (col_num >=(pzinfo->pmat->col)) {
        col_num = 0;
    }
    col[c_end++] = col_num;
    // according  the col nums, get the row nums of parity chunk
    int i, j;

    while (c_start < c_end) {
        for (i = c_start; i < c_end; i++) {
            col_num = col[i];
            for (j = 0; j < (pmat->row); j++) {
                if (pmat->data[j*(pmat->col) + col_num] == 1) {
                    // relative
                    row_num = j;
                    if (find(row, 0, r_end, row_num) < 0) {
                        // not count 
                        row[r_end++] = row_num;
                    }
                }
            }
        }
        c_start = i;
        // according the row nums, get the col nums of the data chunk, and set the list map.
        int line[total], l_end=0;
        for (i = r_start; i < r_end; i++) {
            row_num = row[i];
            l_end = 0;
            for (j = 0; j < (pmat->col); j++) {
                if (pmat->data[row_num*(pmat->col) + j] == 1) {
                    // relative
                    col_num = j;
                    if (find(col, 0, c_end, col_num) < 0) {
                        // not count 
                        if (col_num>= start && col_num <= end) {
                           pos = col_num; 
                        } else {
                            col[c_end++] = col_num;
                        }
                    }
                    line[l_end++] = col_num;
                }
            }
            j = find(line, 0, l_end, pos);
            line[j] = row_num + (pmat->col);
            // set the list map
            if (list[pos-start].len == 0) {
                // not set
                set_list(&(list[pos-start]), l_end, line);
            }
        }
        r_start = i;
    } // while (c_start < c_end)
    return 1;
}
*/

int z_init(z_info_t *pzinfo, int m, int k, int datasize, int packetsize){

    pzinfo->pzlil = (lil_t *)LIL_ALLOC(m, k);
    make_pgm_lil(pzinfo->pzlil, m, k);
    print_matrix(pzinfo->pzlil, LIL_ROW(m,k), LIL_COL(m,k));

    pzinfo->m = m;
    pzinfo->k = k;


    pzinfo->datasize = datasize;
    pzinfo->packetsize = packetsize;

    pzinfo->blocksize = get_block_size(datasize, m, k);

    return LIL_R(m, k);
}

int z_free(z_info_t *pzinfo){

    lil_free(pzinfo->pzlil);
    //free(pzinfo);

    return 1;
}

int z_encode(z_info_t *pzinfo, unsigned char *psrc, unsigned char *pdes){

    // encode 
    mxcoding_dbg(pdes, psrc, pzinfo->blocksize, pzinfo->packetsize, pzinfo->pzlil,  pzinfo->m, pzinfo->k);
    //mxcoding_ppg(pdes, psrc, pzinfo->blocksize, pzinfo->packetsize, pzinfo->pzlil,  pzinfo->m, pzinfo->k);

    return 1;
}

int z_encode_dpg(z_info_t *pzinfo, unsigned char *psrc, unsigned char *pdes){

    // encode 
    mxcoding_dpg(pdes, psrc, pzinfo->blocksize, pzinfo->packetsize, pzinfo->pzlil,  pzinfo->m, pzinfo->k);
    //mxcoding_ppg(pdes, psrc, pzinfo->blocksize, pzinfo->packetsize, pzinfo->pzlil,  pzinfo->m, pzinfo->k);

    return 1;
}

/**
 * repair one node;
 * @param node, the node to be repaired, start from 0 to k-1;
int z_repair(z_info_t *pzinfo, unsigned char *psrc, blist_t *phelp,int node, unsigned char *pdes)
{
    if (node >= pzinfo->k) {
        print_error("ERROR: node num >= k\n");
        return 0;
    }

    // get the relative row nums of each chunk to repair
    int fragment = (pzinfo->pmat->col) / (pzinfo->k); //nums in one node
    //int total = (pzinfo->k + pzinfo->m -1)*fragment/(pzinfo->m); // total rows to repair
    blist_t parity_map[fragment]; // the map between repair chunk num and the repair row nums
    int i, k;
    for (i = 0; i < fragment; i++) {
        init_list(&(parity_map[i]), 0);
    }
    if (get_repair_list(parity_map, pzinfo, node) == 0) {
        print_error("ERROR: get repair list error\n");
        // free memory
        for (k = 0; k < fragment; k++) {
            free_list(&(parity_map[k]));
        }
        return 0;
    }

    // repair each chunk data
    blist_t *list;
    int row, pos, j;
    for (i = 0; i < fragment; i++) {
        list = &parity_map[i];
        for (j = 0; j < list->len; j++) {
            row= list->plist[j];
            pos = find(phelp->plist,0,phelp->len,row);
            if (pos<0) {
                print_error("ERROR: find row error\n");
                // free memory
                for (k = 0; k < fragment; k++) {
                    free_list(&(parity_map[k]));
                }
                return 0;
            }
            region_xor(pdes+i*(pzinfo->blocksize), psrc+pos*(pzinfo->blocksize), pzinfo->blocksize);
        }
    }
    // free memory
    for (k = 0; k < fragment; k++) {
        free_list(&(parity_map[k]));
    }

    return 1;
}
 */
