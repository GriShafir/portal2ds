#ifndef CHELL__H
#define CHELL__H


#define CHELL_BITMAP_WIDTH   128
#define CHELL_BITMAP_HEIGHT  256
#define CHELL_NUM_IMAGES   14


		//	Format:
		//	U,V,Width,Height



const unsigned int chell_texcoords[] = {
 1, 0, 23, 24,				// 0 idle
 1, 26, 24, 24,             // 1 walking
 26, 26, 23, 25,            // 2 walking
 1, 52, 24, 26,             // 3 walking
 26, 52, 30, 29,            // 4 jumping
 79, 0, 20, 24,             // 0 with gun
 75, 26, 22, 23,            // 1 with gun
 74, 51, 28, 22,            // 3 with gun
 104, 25, 19, 25,           // 2 with gun
 104, 51, 24, 30,           // 4 with gun
 106, 0, 20, 24,            // looking up with gun
 78, 82, 21, 24,            // aim with gun
 104, 82, 24, 30,           // aim jump with gun
};


#endif
