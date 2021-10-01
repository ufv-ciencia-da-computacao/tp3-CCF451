#include "../lib/bitmap.h"


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

	set_bits(&b, 0, 129);
	//print(&b);
	reset_bits(&b, 0, 62);
	//print(&b);
	reset_bits(&b, 63, 69);
	print(&b);
	destroy_bitmap(&b);
	return 0;
}
