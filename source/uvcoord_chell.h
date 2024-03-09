#ifndef CHELL__H
#define CHELL__H


#define CHELL_BITMAP_WIDTH   128
#define CHELL_BITMAP_HEIGHT  128
#define CHELL_NUM_IMAGES   10


		//	Format:
		//	U,V,Width,Height



const unsigned int chell_texcoords[] = {
 1, 4, 17, 30,				// (0) idle
 38, 4, 16, 30,             // (1) walking 1
 56, 5, 20, 29,            	// (2) walking 2
 38, 35, 16, 31,			// (3) walking 3
 56, 36, 19, 30,			// (4) walking 4
 1, 36, 17, 30,				// (5) jumping
 20, 4, 16, 30,             // (6) look at player
 20, 36, 15, 30,            // (7) look up
 78, 4, 21, 30,				// (8) crouching
};


#endif
