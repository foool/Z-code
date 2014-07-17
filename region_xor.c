/*
 *  region_xor.c
 *
 *  Perform XORing two aligned regions
 *
 */

#include <assert.h>
#include <nmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>

#include "region_xor.h"

int region_xor(void *dst, void*src, int len)
{
#if AVX_SUPPORT == TRUE
    int k;
    int len256 = len/32;
    __m256i val;

    assert(len%32 == 0);

    __m256i *_buf1 = (__m256i *)src;
    __m256i *_buf2 = (__m256i *)dst;

    for(k = 0; k < len256; ++k){
        val = _mm256_xor_si256(_buf1[k], _buf2[k]);
        _mm256_store_si256(&_buf2[k], val);
    }
#elif SSE_SUPPORT == TRUE
    int k;
    int len128 = len/16;
    
    assert(len%16 == 0);
                
    __m128i *_buf1 = (__m128i*)src; 
    __m128i *_buf2 = (__m128i*)dst; 

    for (k=0; k < len128; k++) {
        _buf2[k] = _mm_xor_si128(_buf1[k], _buf2[k]);
    }
#else
    uint8_t *p8_1 = (uint8_t *)dst;
    uint8_t *p8_2 = (uint8_t *)src;
    uint8_t *pdes;

    pdes = p8_1;

    for(; p8_1 < (pdes+len); ++p8_1, ++p8_2){
        *p8_1 = (*p8_1)^(*p8_2);
    }
#endif
    return len;
}
