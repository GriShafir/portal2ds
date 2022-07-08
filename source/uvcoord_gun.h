#ifndef GUN__H
#define GUN__H


#define GUN_BITMAP_WIDTH   32
#define GUN_BITMAP_HEIGHT  16
#define GUN_NUM_IMAGES   3


		//	Format:
		//	U,V,Width,Height



const unsigned int gun_texcoords[] = {
 0, 0, 11, 8,				// no-portals
 13, 0, 11, 8,             // blue-portal
 0, 9, 11, 8,                // orange-portal
};


#endif
