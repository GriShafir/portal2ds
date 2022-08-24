#ifndef AFPLATE__H
#define AFPLATE__H


#define AFPLATE_BITMAP_WIDTH   64
#define AFPLATE_BITMAP_HEIGHT  128
#define AFPLATE_NUM_IMAGES   10


		//	Format:
		//	U,V,Width,Height



const unsigned int afplate_texcoords[] = {
 0, 0, 32, 13,		       // 0
 32, 0, 32, 13,            // 1
 0, 13, 32, 13,            // 2
 32, 13, 32, 13,           // 3
 0, 26, 32, 13,            // 4
 32, 26, 32, 13,           // 5
 0, 39, 32, 13,            // 6
 32, 39, 32, 13,           // 7
 0, 52, 32, 13,            // 8
 32, 52, 32, 13,           // 9
};


#endif
