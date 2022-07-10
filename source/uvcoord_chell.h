#ifndef CHELL__H
#define CHELL__H


#define CHELL_BITMAP_WIDTH   128
#define CHELL_BITMAP_HEIGHT  256
#define CHELL_NUM_IMAGES   10


		//	Format:
		//	U,V,Width,Height



const unsigned int chell_texcoords[] = {
 1, 0, 23, 24,				// 0 idle
 1, 26, 24, 24,             // 1 walking
 26, 26, 23, 25,            // 2 walking
 1, 53, 24, 26,             // 3 walking
 26, 53, 30, 29,            // 4 jumping
 80, 0, 19, 24,             // 0 with gun
 76, 26, 24, 24,            // 1 with gun
 104, 26, 24, 24,           // 2 with gun
 75, 53, 24, 26,            // 3 with gun
 101, 53, 28, 29,           // 4 with gun
};


#endif
