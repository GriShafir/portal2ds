#ifndef CUBE__H
#define CUBE__H


#define CUBE_BITMAP_WIDTH   32
#define CUBE_BITMAP_HEIGHT  64
#define CUBE_NUM_IMAGES   8


		//	Format:
		//	U,V,Width,Height



const unsigned int cube_texcoords[] = {
 0, 0, 11, 11,		     // weigh
 12, 0, 11, 11,          // comp
 0, 12, 11, 11,          // laser1
 0, 24, 11, 11,          // cont1
 12, 36, 11, 11,         // ball
 12, 12, 11, 11,         // laser2
 0, 36, 11, 11,          // cont2
 12, 24, 11, 11,         // cont3
};


#endif
