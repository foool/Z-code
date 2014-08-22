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
    unsigned char *pdata, *psrc, *pdes, *pdes2;

    if (argc>1 && !strcmp(argv[1], "-h")) {
        help(argv[0]);
        return 0;
    }

    if (argc>1) m = atoi(argv[1]);
    if (argc>2) k = atoi(argv[2]);
    if (argc>3) node = atoi(argv[3]);
    if (argc>4) datasize = atoi(argv[4])*1024*1024; // MB
    if (argc>5) packetsize = atoi(argv[5])*1024; // KB

    printf("data size : %.1fMB  \n", (datasize*1.0)/(1024.0*1024.0));
    printf("packet size:%.1fKB  \n", (packetsize*1.0)/(1024.0));

    int r;
    // test zcode init
    r = z_init(&zcode, m, k, datasize, packetsize);
    printf("main block_size : %d\n",zcode.blocksize);

    // init data 
    pdata = (unsigned char *)malloc((2*m+k)*r*zcode.blocksize);

    // test zcode encode
    printf("Encode zcode:\n");
    psrc = pdata;
    pdes = pdata + (k*r*(zcode.blocksize));
    pdes2 = pdes + (m*r*(zcode.blocksize));
    memset(pdes, 0, 2*m*r*(zcode.blocksize));
    z_encode(&zcode, psrc, pdes);
    z_encode(&zcode, psrc, pdes2);

    if(memcmp(pdes, pdes2, m*r*zcode.blocksize) == 0){
        printf("equal!\n");
    }else{
        printf("not equal \n");
    }
    z_free(&zcode);
    free(pdata);

    return 0;
}
