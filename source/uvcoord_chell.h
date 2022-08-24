#ifndef CHELL__H
#define CHELL__H


#define CHELL_BITMAP_WIDTH   128
#define CHELL_BITMAP_HEIGHT  256
#define CHELL_NUM_IMAGES   13


		//	Format:
		//	U,V,Width,Height



const unsigned int chell_texcoords[] = {
 1, 0, 23, 24,				// 0 idle
 1, 26, 24, 24,             // 1 walking
 26, 26, 23, 25,            // 2 walking
 1, 52, 24, 26,             // 3 walking
 26, 52, 30, 29,            // 4 jumping
 79, 0, 20, 24,             // 0 with gun
 75, 26, 25, 24,            // 1 with gun
 104, 25, 24, 26,           // 2 with gun
 75, 51, 25, 26,            // 3 with gun
 102, 51, 27, 30,           // 4 with gun
 106, 0, 20, 24,            // looking up with gun
 79, 82, 23, 24,            // aim with gun
 102, 82, 27, 30,           // aim jump with gun
};


#endif
