#include "includes/bitmap.h"

/*
	All position values and indexes below are 0-indexed
*/

static ull rl(int l, int r){
	ull x = 1;
	return ((x << (r - l + 1)) - 1) << l;
}

void init_bitmap(bitmap_t *bitmap){
	bitmap->bits = (*ull) malloc(BITMAP_SIZE * sizeof(ull));
	memset(bitmap->bits, 0, sizeof(bitmap->bits));
}

void set_bits(bitmap_t *bitmap, int posl, int posr){
	int idx_l = posl >> LN_BIT_SIZE, idx_r = posr >> LN_BIT_SIZE;
	posl %= BIT_SIZE;
	posr %= BIT_SIZE;


	for(int i = idx_l; i <= idx_r; ++i) 
		bitmap->bits[i] |= (i ^ idx_r ? rl(posl, BITMAP_SIZE - 1) : rl(posl, posr)), posl = 0;
}

void reset_bits(bitmap_t *bitmap, int posl, int posr){
	int idx_l = posl >> LN_BIT_SIZE, idx_r = posr >> LN_BIT_SIZE;
	posl %= BIT_SIZE;
	posr %= BIT_SIZE;

	for(int i = idx_l; i <= idx_r; ++i) 
		bitmap->bits[i] ^= (i ^ idx_r ? rl(posl, BITMAP_SIZE - 1) : rl(posl, posr)), posl = 0;
}

int available_blocks(bitmap_t *bitmap){
	int free_blocks = 0;
	for(int i = 0; i < BITMAP_SIZE; ++i) free_blocks += BIT_SIZE - __builtin_popcountll(bitmap->bits[i]);

	return free_blocks;
}

int next_available_block(bitmap_t *bitmap){
	int next_block = -1;
	
	for(int i = 0; i < BITMAP_SIZE; ++i) next_block += BIT_SIZE - __builtin_ctzll(bitmap->bits[i]);

	return next_block;
}

void destroy_bitmap(bitmap_t *bitmap){
	free(bitmap->bits);
	bitmap->bits = NULL;
	free(bitmap);
	bitmap=NULL;
}
