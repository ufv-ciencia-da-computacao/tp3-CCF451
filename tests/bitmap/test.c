#include "../../lib/bitmap.h"

void print(bitmap_t *bitmap){
	ull x = 1;
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < BIT_SIZE; ++j) printf("%d", (x << j) & bitmap->bits[i] ? 1: 0);
		printf("\n");
	}
	printf("\n");
}

int main(){
	bitmap_t b;

	init_bitmap(&b, BITMAP_SIZE);

	set_bits(&b, 0, 1024 * 64 - 1);
	print(&b);
	int n = next_available_block(&b);
	printf("%d\n", n);
	reset_bits(&b, 0, n);
	print(&b);
	set_bits(&b, 0, 69);
	print(&b);
	printf("%d\n", next_available_block(&b));
	destroy_bitmap(&b);
	return 0;
}
