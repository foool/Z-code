#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../zcode.h"

void help(const char *file)
{
    printf("Usage: %s [m k node datasize packetsize]\n\n", file);
    printf("  m:\t\t parity devices number(2)\n");
    printf("  k:\t\t data devices number(3)\n");
    printf("  node:\t\t repair node number(1)\n");
    printf("  datasize:\t the  data size(MB)(512M)\n");
    printf("  packetsize:\t the process unit(KB)(2KB)\n");
}

int main(int argc, char const* argv[])
{
    z_info_t zcode;

    int m = 2;  // init to 2, parity deveices number
    int k = 3;  // init to 3, data devices number
    int node = 1;  // init to 1, repair node number
    int datasize = 512*1024*1024; // init to 512M, file size
    int packetsize = 2*1024; // init to 2KB, process unit 
    unsigned char *pdata, *psrc, *pdes;

    if (argc>1 && !strcmp(argv[1], "-h")) {
        help(argv[0]);
        return 0;
    }

    if (argc>1) m = atoi(argv[1]);
    if (argc>2) k = atoi(argv[2]);
    if (argc>3) node = atoi(argv[3]);
    if (argc>4) datasize = atoi(argv[4])*1024*1024; // MB
    if (argc>5) packetsize = atoi(argv[5])*1024; // KB

    int ret;
    // test zcode init
    printf("Init zcode:\n");
    ret = z_init(&zcode, m, k, datasize, packetsize);
    printf("init ret: %d\n", ret);

    // init data 
    pdata = (unsigned char *)malloc((zcode.pmat->col+zcode.pmat->row)* zcode.blocksize);

    // test zcode encode
    printf("Encode zcode:\n");
    psrc = pdata;
    pdes = pdata + (zcode.pmat->col)*(zcode.blocksize);
    ret = z_encode(&zcode, psrc, pdes);
    printf("encode ret: %d\n", ret);

    // test zcode repair
    printf("Repair:\n");
    blist_t help;
    help.len = zcode.pmat->col+zcode.pmat->row;
    help.plist = (int*)malloc(sizeof(int)*(help.len));
    int i;
    for (i = 0; i < help.len; i++) {
        help.plist[i] = i;
    }
    psrc = pdata;
    pdes = pdata + node * (zcode.blocksize) * (zcode.pmat->col)/k;
    ret = z_repair(&zcode, psrc, &help, node, pdes);
    printf("num = %d , repair ret: %d\n", node, ret);

    z_free(&zcode);
    free(pdata);
    free(help.plist);

    return 0;
}
