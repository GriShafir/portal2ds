#ifndef CONNECT__H
#define CONNECT__H


#define CONNECT_BITMAP_WIDTH   32
#define CONNECT_BITMAP_HEIGHT  32
#define CONNECT_NUM_IMAGES   9


		//	Format:
		//	U,V,Width,Height



const unsigned int connect_texcoords[] = {
 0, 0, 9, 9,				// 0
 10, 0, 9, 9,               // 1
 19, 0, 9, 9,               // 2
 0, 10, 9, 9,               // 3
 10, 10, 9, 9,              // 4
 19, 10, 9, 9,              // 5
 0, 19, 9, 9,               // 6
 10, 19, 9, 9,              // 7
 19, 19, 9, 9,              // 8
};


#endif
