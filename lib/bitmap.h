#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT_SIZE (1 << 6)
#define BITMAP_SIZE (1 << 10)

#define LN_BIT_SIZE 6
#define LN_BITMAP_SIZE 10

typedef unsigned long long ull;
typedef struct bitmap{
	ull *bits;
} bitmap_t;

void init_bitmap(bitmap_t *bitmap, int size);
void set_bits(bitmap_t *bitmap, int posl, int posr);
void reset_bits(bitmap_t *bitmap, int posl, int posr);
int available_blocks(bitmap_t *bitmap);
int next_available_block(bitmap_t *bitmap);
void destroy_bitmap(bitmap_t *bitmap);

#endif
