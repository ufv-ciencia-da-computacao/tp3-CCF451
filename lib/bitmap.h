#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ULL_SIZE (1 << 6)
#define BYTE_SIZE (1 << 3)
#define LN_ULL_SIZE 6
#define LN_BYTE_SIZE 3

typedef unsigned long long ull;
typedef unsigned int uint;
typedef struct bitmap{
	ull *bits;
	int size;
} bitmap_t;

void init_bitmap(bitmap_t *bitmap, uint size);
void set_bits(bitmap_t *bitmap, int posl, int posr);
void reset_bits(bitmap_t *bitmap, int posl, int posr);
int get_bit(bitmap_t *bitmap, int pos);
int available_blocks(bitmap_t *bitmap);
int next_available_block(bitmap_t *bitmap);
void destroy_bitmap(bitmap_t *bitmap);

#endif
