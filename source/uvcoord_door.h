#ifndef DOOR__H
#define DOOR__H


#define DOOR_BITMAP_WIDTH   128
#define DOOR_BITMAP_HEIGHT  128
#define DOOR_NUM_IMAGES   10


		//	Format:
		//	U,V,Width,Height



const unsigned int door_texcoords[] = {
 0, 0, 29, 26,		       // 0
 30, 0, 29, 26,            // 1
 60, 0, 29, 26,            // 2
 90, 0, 29, 26,            // 3
 0, 27, 29, 26,            // 4
 30, 27, 29, 26,           // 5
 60, 27, 29, 26,           // 6
 90, 27, 29, 26,           // 7
 0, 54, 29, 26,            // 8
 30, 54, 29, 26,           // 9
};


#endif
