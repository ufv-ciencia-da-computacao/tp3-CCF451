#include "../lib/bitmap.h"

/*
	All position values and indexes below are 0-indexed
	size parameter must be a multiple of 64

*/

static ull rl(int l, int r){
	ull x = 1;
	int ok = (r - l + 1) ^ ULL_SIZE;
	int interval = (ok ? r - l + 1 : r - l);

	ull ans = ((x << (interval)) - 1) << (ok ? l : l + 1);

	if(!ok) ans |= x;
	return ans;
}

static int sz(int size){
	int length = (size % ULL_SIZE ? (size >> LN_ULL_SIZE) + 1: size >> LN_ULL_SIZE)	;
	return length;
}

void init_bitmap(bitmap_t *bitmap, uint size){
	bitmap->size = size;
	bitmap->bits = (ull*) malloc(sz(size) * sizeof(ull));

	memset(bitmap->bits, 0, sizeof(bitmap->bits));
}

void init_bitmap_array(bitmap_t *bitmap, void *array, uint size){
	bitmap->size = size;
	memcpy(bitmap->bits, array, BYTE_SIZE * bitmap->size);
}


void set_bits(bitmap_t *bitmap, int posl, int posr){
	if(posr > bitmap->size - 1){
		printf("Right position value should be less or equal bitmap->size - 1");
		return;
	}
	int idx_l = posl >> LN_ULL_SIZE, idx_r = posr >> LN_ULL_SIZE;
	posl %= ULL_SIZE;
	posr %= ULL_SIZE;


	for(int i = idx_l; i <= idx_r; ++i)
		bitmap->bits[i] |= (i ^ idx_r ? rl(posl, ULL_SIZE - 1) : rl(posl, posr)), posl = 0;
}

void reset_bits(bitmap_t *bitmap, int posl, int posr){
	if(posr > bitmap->size - 1){
		printf("Right position value should be less or equal bitmap->size - 1");
		return;
	}

	int idx_l = posl >> LN_ULL_SIZE, idx_r = posr >> LN_ULL_SIZE;
	posl %= ULL_SIZE;
	posr %= ULL_SIZE;

	for(int i = idx_l; i <= idx_r; ++i)
		bitmap->bits[i] &= ~(i ^ idx_r ? rl(posl, ULL_SIZE - 1) : rl(posl, posr)), posl = 0;
}

int available_blocks(bitmap_t *bitmap){
	int free_blocks = 0;
	int length = sz(bitmap->size);

	for(int i = 0; i < length; ++i) free_blocks += ULL_SIZE - __builtin_popcountll(bitmap->bits[i]);

	free_blocks -= (ULL_SIZE - bitmap->size % ULL_SIZE);

	return free_blocks;
}

int next_available_block(bitmap_t *bitmap){
	int next_block = 0;

	for(int i = 0; i < sz(bitmap->size) && bitmap->bits[i]; ++i)
		next_block += ULL_SIZE - __builtin_clzll(bitmap->bits[i]);

	return (next_block >= bitmap->size? -1 : next_block);
}

void destroy_bitmap(bitmap_t *bitmap){
	free(bitmap->bits);
	bitmap->bits = NULL;
	bitmap->size = -1;
}
