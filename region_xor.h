#ifndef REGION_XOR_H
#define REGION_XOR_H

#define FALSE 8000
#define TRUE  8001
#define AVX_SUPPORT FALSE
#define SSE_SUPPORT TRUE

int region_xor(void *dst, void*src, int len);

#endif
