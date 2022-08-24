/*------------------------------------------------------------------------------ 
portal2ds by GriShafir

Suppositioned controls:
UP + X = Make blue portal up
DOWN + X = Make blue portal down
UP + Y = Make orange portal up
DOWN + Y = Make orange portal down

B = press the button (if one is near)
R = Grab an object (for example, cube or turret)
DOWN = Crouch
------------------------------------------------------------------------------ */

#include <nds.h>
#include <stdio.h>
#include <gl2d.h>
#include <array>

// Auto-generated GRIT files
#include "chell.h"
#include "gun.h"
#include "blueportal.h"
#include "orangeportal.h"
#include "cubes.h"
#include "cubedropper.h"
#include "floorbutton.h"
#include "pillarbutton.h"
#include "door.h"
#include "aerialfaithplate.h"
#include "tile.h"
#include "check.h"
#include "connect.h"
#include "wheatley.h"
#include "warning.h"

// UVCOORD files (I wrote them myself)
#include "uvcoord_chell.h"
#include "uvcoord_gun.h"
#include "uvcoord_blportal.h"
#include "uvcoord_orportal.h"
#include "uvcoord_cubes.h"
#include "uvcoord_flbutton.h"
#include "uvcoord_pibutton.h"
#include "uvcoord_door.h"
#include "uvcoord_afplate.h"
#include "uvcoord_check.h"
#include "uvcoord_connect.h"
#include "uvcoord_wheatley.h"

glImage  Chell[CHELL_NUM_IMAGES];	
glImage  Gun[GUN_NUM_IMAGES];	 

glImage  BlPortal[BLPORTAL_NUM_IMAGES];
glImage  OrPortal[ORPORTAL_NUM_IMAGES];

glImage  CubeID[CUBE_NUM_IMAGES];
glImage  CDropper[1];
glImage  FlButton[FLBUTTON_NUM_IMAGES];
glImage  PiButton[PIBUTTON_NUM_IMAGES];
glImage  Door[DOOR_NUM_IMAGES];
glImage  Check[CHECK_NUM_IMAGES];
glImage  Connect[CONNECT_NUM_IMAGES];
glImage  AFPlate[AFPLATE_NUM_IMAGES];

glImage  Wheatley[WHEATLEY_NUM_IMAGES];

glImage  Tile[1];
glImage  Warn[1];

int floorHeight = SCREEN_HEIGHT - 28;

int playerSize[2] = {23, 24};
int playerPos[2] = {SCREEN_WIDTH / 2 - playerSize[0], floorHeight - playerSize[1] - 100};
int facing = 1; // 0 - left; 1 - right

int world_move = 0; // the camera thingy

class Portal {
    public:
        int side; // 0 - left, 1 - right
        std::array<int, 2> pos; // {x1, y1}

        int textureID;
};

class Cube {
    public: 
        int type; // 0 - weighted, 1 - companion, 2 - laser, 3 - contraption
        std::array<int, 2> pos; // {x1, y1}

        float speed;
};

class Platform {
    public:
        int type; // 0 - non-portable, 1 - portable
        std::array<int, 4> pos; // {x1, y1, x2, y2}

    int collide_top() const
    {
        Platform g;
        g = *this;  

        if ((playerPos[1] < g.pos[1] - playerSize[1]) 
        && ((playerPos[0] < g.pos[2]) && (playerPos[0] > g.pos[0] - playerSize[0]))) {
            return 0; // no collide on the top of g

        } else if ((playerPos[1] > g.pos[1]) 
        && ((playerPos[0] < g.pos[2]) && (playerPos[0] > g.pos[0] - playerSize[0]))) {
            return 0; // no collide on the bottom of g
        } else {
            return 1; // yes collide
        } 

        /* if ((playerPos[0] < g.pos[0] + (g.pos[2] - g.pos[0])) &&
        (playerPos[0] + playerSize[0] > g.pos[0]) &&
        (playerPos[1] > g.pos[1]) &&
        (playerPos[1] + playerSize[1] < g.pos[3])) return 1;
        else return 0; */
    }

    int collide_side() const 
    {
        Platform g;
        g = *this;

        if ((playerPos[0] < g.pos[0] + (g.pos[2] - g.pos[0])) &&
        (playerPos[0] + playerSize[0] > g.pos[0]) &&
        (playerPos[1] > g.pos[1]) &&
        (playerPos[1] + playerSize[1] < g.pos[3])) return 1;
        else return 0;
    }

    void draw() const
    {
        Platform g;
        g = *this;
        glBoxFilled(g.pos[0], g.pos[1], g.pos[2], g.pos[3], RGB15(219, 219, 219));
    }
};

int player_pos[2] = {0, 0};
int world_size[2] = {384, 288};

int main() {
    /* sprites */
    videoSetMode(MODE_5_3D);
    consoleDemoInit();
    glScreen2D();

    vramSetBankA( VRAM_A_TEXTURE );
	vramSetBankB( VRAM_B_TEXTURE );

	vramSetBankF(VRAM_F_TEX_PALETTE);

    int Frame = 0;

    float playerHorSpeed = 0;
    float playerSpeed = 0;
    int jumped = 0;
    /* int hasGun = 1; // 0 - player doesn't have, 
                       // 1 - player has*/
    int isHolding = 0; // 0 - no, 1 - yes

    int ChellTextureID = 
	glLoadSpriteSet( Chell,					// pointer to glImage array
					 CHELL_NUM_IMAGES, 		// Texture packer auto-generated #define
					 chell_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_128,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_256,		// sizeY for glTexImage2D() in videoGL.h
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)chellPal,		// Load our 256 color player palette
					 (u8*)chellBitmap	 	// image data generated by GRIT
				   );

    int ChellFlip = GL_FLIP_NONE;

    int GunTextureID = 
	glLoadSpriteSet( Gun,
					 GUN_NUM_IMAGES,
					 gun_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_32,
					 TEXTURE_SIZE_16,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)gunPal,
					 (u8*)gunBitmap
				   );
    int ChellFrame = 5;
    int GunFrame = 0;
    int PortalFrame = 0;
    int DoorFrame = 0;
    int AFPlateFrame = 0;
    int ConnectFrame = 0;

    Portal port1;
    Portal port2;

    int BlPortalTextureID = 
	glLoadSpriteSet( BlPortal,
					 BLPORTAL_NUM_IMAGES,
					 blportal_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_64,
					 TEXTURE_SIZE_64,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)blueportalPal,
					 (u8*)blueportalBitmap
				   );

    port1.pos[0] = -500;
    port1.pos[1] = -500;
    port1.side = 1;
    port1.textureID = BlPortalTextureID;

    int port1Flip = GL_FLIP_H;

    int port1_pos[2] = {-500, -500};

    int OrPortalTextureID = 
	glLoadSpriteSet( OrPortal,
					 ORPORTAL_NUM_IMAGES,
					 orportal_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_64,
					 TEXTURE_SIZE_64,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)orangeportalPal,
					 (u8*)orangeportalBitmap
				   );

    port2.pos[0] = -500;
    port2.pos[1] = -500;
    port2.side = 0;
    port2.textureID = OrPortalTextureID;
    int port2Flip = GL_FLIP_H;

    int port2_pos[2] = {-500, -500};

    int CDropperTextureID = 
	glLoadTileSet( CDropper,
				     19,
					 22,
					 19,
					 22,
					 GL_RGB256,
					 TEXTURE_SIZE_32,
					 TEXTURE_SIZE_32,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)cubedropperPal,
					 (u8*)cubedropperBitmap
				   );

    std::array<int, 2> cDropperPos = {SCREEN_WIDTH - 26, 0};

    Cube cube;
    int CubeTextureID = 
	glLoadSpriteSet( CubeID,
					 CUBE_NUM_IMAGES,
					 cube_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_32,
					 TEXTURE_SIZE_64,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)cubesPal,
					 (u8*)cubesBitmap
				   );


    cube.pos = {cDropperPos[0] + 4, cDropperPos[1] + 10};

    cube.speed = 0;
    cube.type = 1;

    int FlButtonTextureID = 
	glLoadSpriteSet( FlButton,
					 FLBUTTON_NUM_IMAGES,
					 flbutton_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_32,
					 TEXTURE_SIZE_16,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)floorbuttonPal,
					 (u8*)floorbuttonBitmap
				   );

    int flButtonPos[2] = {SCREEN_WIDTH - 206, floorHeight - 4};
    int isActive = 0;

    int DoorTextureID = 
	glLoadSpriteSet( Door,
					 DOOR_NUM_IMAGES,
					 door_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_128,
					 TEXTURE_SIZE_128,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)doorPal,
					 (u8*)doorBitmap
				   );

    int doorPos[2] = {SCREEN_WIDTH - 76, floorHeight - 27};

    int CheckTextureID = 
	glLoadSpriteSet( Check,
					 CHECK_NUM_IMAGES,
					 check_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_32,
					 TEXTURE_SIZE_16,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)checkPal,
					 (u8*)checkBitmap
				   );

    int checkX = doorPos[0] - 11;

    int ConnectTextureID = 
	glLoadSpriteSet( Connect,
					 CONNECT_NUM_IMAGES,
					 connect_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_32,
					 TEXTURE_SIZE_32,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)connectPal,
					 (u8*)connectBitmap
				   );

    int connectPos[4] = {doorPos[0] - 11, doorPos[1] - 9, flButtonPos[0] + 3, flButtonPos[1] - 10};

    int AFPlateTextureID = 
	glLoadSpriteSet( AFPlate,
					 AFPLATE_NUM_IMAGES,
					 afplate_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_64,
					 TEXTURE_SIZE_128,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)aerialfaithplatePal,
					 (u8*)aerialfaithplateBitmap
				   );

    int AFPlatePos[2] = {SCREEN_WIDTH - 126, floorHeight - 12};
    int plateJumped = 0;

    int WheatleyTextureID = 
	glLoadSpriteSet( Wheatley,
					 WHEATLEY_NUM_IMAGES,
					 wheatley_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_64,
					 TEXTURE_SIZE_64,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)wheatleyPal,
					 (u8*)wheatleyBitmap
				   );

    int WheatleyPos[2] = {100, 100};
    int WheatleyFrame = 0;
    int WheFlip = GL_FLIP_H;

    int PiButtonTextureID = 
	glLoadSpriteSet( PiButton,
					 PIBUTTON_NUM_IMAGES,
					 pibutton_texcoords,
					 GL_RGB256,
					 TEXTURE_SIZE_16,
					 TEXTURE_SIZE_16,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)pillarbuttonPal,
					 (u8*)pillarbuttonBitmap
				   );

    int piButtonPos[2] = {25, floorHeight - 14};
    int isPressed = 0;

    int TileTextureID = 
	glLoadTileSet( Tile,
				     16,
					 16,
					 16,
					 16,
					 GL_RGB256,
					 TEXTURE_SIZE_16,
					 TEXTURE_SIZE_16,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)tilePal,
					 (u8*)tileBitmap
				   );

    int WarnTextureID = 
	glLoadTileSet( Warn,
				     128,
					 32,
					 128,
					 32,
					 GL_RGB256,
					 TEXTURE_SIZE_128,
					 TEXTURE_SIZE_32,
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					 256,
					 (u16*)warningPal,
					 (u8*)warningBitmap
				   );

    int warnX = 0;

    Platform wall1;
    wall1.type = 1;
    wall1.pos = {-(world_size[0] / 2), 164, world_size[0] * 1.5, 180};

    Platform wall2;
    wall2.type = 1;
    wall2.pos = {-(world_size[0] / 2), 0, -(world_size[0] / 2) + 16, SCREEN_HEIGHT - 12};

    Platform wall3;
    wall3.type = 1;
    wall3.pos = {world_size[0] + 49, 0, world_size[0] + 65, SCREEN_HEIGHT - 12};

    /* game logic */
    while (1) {

        Frame++;

        scanKeys();

        glBegin2D();

        // DrawBG(Tile);

        glSprite(flButtonPos[0], flButtonPos[1], GL_FLIP_NONE, &FlButton[isActive]);  // floor button
        glSprite(piButtonPos[0], piButtonPos[1], GL_FLIP_NONE, &PiButton[isPressed]); // pillar button
        glSprite(doorPos[0], doorPos[1], GL_FLIP_NONE, &Door[DoorFrame]);             // door
        glSprite(checkX, doorPos[1], GL_FLIP_NONE, &Check[isActive]);                 // check
        glSprite(connectPos[0], connectPos[1], GL_FLIP_NONE, &Connect[ConnectFrame]); // connect (??) 1
        glSprite(connectPos[2], connectPos[3], GL_FLIP_NONE, &Connect[ConnectFrame]); // connect (??) 2
        glSprite(cube.pos[0], cube.pos[1], GL_FLIP_NONE, &CubeID[cube.type]);         // cube
        glSprite(cDropperPos[0], cDropperPos[1], GL_FLIP_NONE, &CDropper[0]);         // cube dropper
        glSprite(AFPlatePos[0], AFPlatePos[1], GL_FLIP_NONE, &AFPlate[AFPlateFrame]); // aerial faith plate

        glSprite(WheatleyPos[0], WheatleyPos[1], WheFlip, &Wheatley[WheatleyFrame]);  // whealtey

        glSprite(port1.pos[0], port1.pos[1], port1Flip, &BlPortal[PortalFrame]);      // blue portal
        glSprite(port2.pos[0], port2.pos[1], port2Flip, &OrPortal[PortalFrame]);  // orange portal

        glSprite(playerPos[0], playerPos[1], ChellFlip, &Chell[ChellFrame]);          // chell

        glSprite(warnX, 100, GL_FLIP_NONE, &Warn[0]);                                 // portal no work

        if (facing == 0) ChellFlip = GL_FLIP_H;
        else if (facing == 1) ChellFlip = GL_FLIP_NONE;
        
        // glBoxFilled(playerPos[0], playerPos[1], playerPos[0] + playerSize[0], playerPos[1] + playerSize[1], RGB15(218, 255, 255)); // drawing player box
        // glBoxFilled(gunPosX, gunPosY, gunPosX + 12, gunPosY + 7, RGB15(221, 221, 221)); // drawing portal gun box

        // glBoxFilled(cubePosX, cubePosY, cubePosX + size, cubePosY + size, RGB15(221, 221, 221)); // drawing cube

        // glBoxFilled(port1.pos[0], port1.pos[1], port1.pos[0] + 4, port1.pos[1] + 34, RGB15(44, 68, 254)); // blue portal
        // glBoxFilled(port2.pos[0], port2.pos[1], port2.pos[0] + 4, port2.pos[1] + 34, RGB15(255, 137, 44)); // orange portal

        wall1.draw();
        wall2.draw();
        wall3.draw();


        iprintf("\x1b[2J");
        iprintf("portal2ds v0.2.3-ge - Made by \nGriShafir\n\nA Portal game in 2D made for \nNintendo DS(i).\n\n"); // desc

        iprintf("To move around, press Left and \nRight on the D-Pad.\n");               // controls
        iprintf("To jump, press A.\nTo grab an object, press B near that object.\n");
        iprintf("To create blue portal, press X.\nTo create orange portal, press \nY.");

        iprintf("\nPlayer's coordinates: \n%i, %i; %i\n", player_pos[0], playerPos[1], facing);        // coords
        iprintf("\nWorld move: %i", world_move);
        iprintf("\nWorld size: %ix%i", world_size[0], world_size[1]);
        iprintf("\nPlayer's collide: %i, %i, %i \n(0 - no, 1 - yes)", wall1.collide_top(), wall2.collide_side(), wall3.collide_side());
        /* iprintf("\nBlue portal's coordinates: \n%i, %i\n", port1.pos[0], port1.pos[1]);
        iprintf("\nOrange portal's coordinates: \n%i, %i\n", port2.pos[0], port2.pos[1]); */

        if ((KEY_A & keysDown()) & (jumped == 0)) { // basic jumping
            playerSpeed = -1.75;
            jumped = 1;
        } else if (KEY_LEFT & keysHeld()) { // basic walking
            world_move = 2;
            facing = 0;

            if ((Frame & 5) == 0) {
                ChellFrame++;

                if (ChellFrame > 8) ChellFrame = 6;
            }
        } else if (KEY_RIGHT & keysHeld()) {
            world_move = -2;
            facing = 1;

            if ((Frame & 5) == 0) {
                ChellFrame++;

                if (ChellFrame > 8) ChellFrame = 6;
            }
        } else if (KEY_UP & keysHeld()) {
            ChellFrame = 10;
            world_move = 0;
        } else {
            ChellFrame = 5;
            world_move /= 2;

            if (world_move < .0005) {
                world_move = 0;
            }
        } 

        if ((WheatleyPos[0] - playerPos[0]) <= 0) {
            WheFlip = GL_FLIP_NONE;
        } else {
            WheFlip = GL_FLIP_H;
        }
        
        if ((KEY_X & keysDown()) && (isHolding == 0)) { // creating blue portal (requires rewrite)
            if (facing == port2.side) {
                if (!(playerPos[1] - 5 >= port2.pos[1] && playerPos[1] - 5 <= port2.pos[1] + 29) && 
                    !(playerPos[1] + 24 <= port2.pos[1] + 29 && playerPos[1] + 24 >= port2.pos[1])) {

                    port1.pos[1] = playerPos[1] - 5;

                    switch(facing) {
                        case 1:
                            port1.pos[0] = wall3.pos[0] - 5;
                            port1Flip = GL_FLIP_H;
                            break;
                        default:
                            port1.pos[0] = wall2.pos[2] + 1;
                            port1Flip = GL_FLIP_NONE;
                            break;
                    }
                }
            }
            else {
                port1.pos[1] = playerPos[1] - 5;

                    switch(facing) {
                        case 1:
                            port1.pos[0] = wall3.pos[0] - 5;
                            port1Flip = GL_FLIP_H;
                            break;
                        default:
                            port1.pos[0] = wall2.pos[2] + 1;
                            port1Flip = GL_FLIP_NONE;
                            break;
                    }
            } 
        } else if ((KEY_Y & keysDown()) && (isHolding == 0)) { // creating orange portal 
            if (facing == port1.side) {
                if (!(playerPos[1] - 5 >= port1.pos[1] && playerPos[1] - 5 <= port1.pos[1] + 29) && 
                    !(playerPos[1] + 24 <= port1.pos[1] + 29 && playerPos[1] + 24 >= port1.pos[1])) {

                    port2.pos[1] = playerPos[1] - 5;

                    switch(facing) {
                        case 1:
                            port2.pos[0] = wall3.pos[0] - 5;
                            port2Flip = GL_FLIP_H;
                            break;
                        default:
                            port2.pos[0] = wall2.pos[2] + 1;
                            port2Flip = GL_FLIP_NONE;
                            break;
                    }
                }
            }
            else {
                port2.pos[1] = playerPos[1] - 5;

                    switch(facing) {
                        case 1:
                            port2.pos[0] = wall3.pos[0] - 5;
                            port2Flip = GL_FLIP_H;
                            break;
                        default:
                            port2.pos[0] = wall2.pos[2] + 1;
                            port2Flip = GL_FLIP_NONE;
                            break;
                    }
            } 
        }

        if ((KEY_B & keysDown()) // holding a cube
        && ((cube.pos[0] - playerPos[0] < 30) && (cube.pos[0] - playerPos[0] > -30)) 
        && ((playerPos[0] - cube.pos[0] < 30) && (playerPos[0] - cube.pos[0] > -30))
        && ((playerPos[1] - cube.pos[1] < 30) && (playerPos[1] - cube.pos[1] > -30))) {
            switch(isHolding) {
                case 0:
                    isHolding = 1;
                    break;
                default:
                    isHolding = 0;
                    break;
            }
        }

        if (isHolding == 1) { // holding a cube 2
            cube.pos[1] = playerPos[1] + 7;
            cube.speed = playerSpeed;
            switch(facing) {
                case 1:
                    cube.pos[0] = playerPos[0] + 25;
                    break;
                default:
                    cube.pos[0] = playerPos[0] - 15;
                    break;
            }
        }

        if ((cube.pos[0] > flButtonPos[0]) && (cube.pos[0] + 11 < flButtonPos[0] + 25) // pressing on floor button
        && (cube.pos[1] + 11 > flButtonPos[1] - 1)) {
            isActive = 1;
        } else {
            isActive = 0;
        }

        if (((piButtonPos[0] - playerPos[0] < 30) && (piButtonPos[0] - playerPos[0] > -30)) // pressing on pillar button
        && ((playerPos[0] - piButtonPos[0] < 30) && (playerPos[0] - piButtonPos[0] > -30))
        && ((playerPos[1] - piButtonPos[1] < 30) && (playerPos[1] - piButtonPos[1] > -30)) 
        && (KEY_B & keysDown()) && (isHolding == 0)) isPressed = 1;

        if (isPressed == 1) { // release the cube from the jaws of the cube dropper
            cube.pos[1] = cDropperPos[1] + 33;

            if ((Frame & 150) == 0) {
                isPressed = 0;
            }
        }

        if ((cube.pos[1] + 11 < cDropperPos[1] + 22) // checking if cube is in dropper
        && (cube.pos[0] > cDropperPos[0]) && (cube.pos[0] + 11 < cDropperPos[0] + 19)) {
            cube.speed = 0;
        }

        switch(isActive) { // opening door
            case 1:
                if (DoorFrame < 5) {
                    if ((Frame & 7) == 0) {
                        DoorFrame++;
                    }
                } else {
                    if ((Frame & 3) == 0) {
                        DoorFrame++;
                    }
                    if (DoorFrame > 9) DoorFrame = 9;
                }
                
                cube.pos[1] = flButtonPos[1] - 10;
                cube.speed = 0;
                break;
            default:
                if ((Frame & 2) == 0) {
                    DoorFrame--;
                    
                    if (DoorFrame < 0) DoorFrame = 0;
                }
        }

        if ((DoorFrame == 9) && (KEY_UP & keysDown()) // going in the door
        && ((playerPos[0] > doorPos[0]) && (playerPos[0] + playerSize[0] < doorPos[0] + 29) && (playerPos[1] > doorPos[1]))) {
            ChellFrame = 5;
            break;
        }

        if ((Frame & 5) == 0) { // portal moving
            PortalFrame++;

            if (PortalFrame > 5) {
                PortalFrame = 0;
            } 
        } 

        if ((playerPos[0] > AFPlatePos[0] + 3) && (playerPos[0] + playerSize[0] < AFPlatePos[0] + 45) // jumping on an aerial faith plate
        && (plateJumped == 0) && (playerPos[1] + 24 == AFPlatePos[1] + 12)) {
            plateJumped = 1;

            playerHorSpeed = 0;
            playerSpeed = -4;
        }

        switch(plateJumped) { // aerial faith plate animation
            case 1:
                if ((Frame & 1) == 0) {
                    AFPlateFrame++;

                    if (AFPlateFrame > 9) {
                        AFPlateFrame = 9;

                        plateJumped = 0;
                    }
                }
                break;
            default:
                if ((Frame & 7) == 0) {
                    AFPlateFrame--;

                    if (AFPlateFrame < 0) AFPlateFrame = 0;
                }
                break;
        }
        
        if ((playerPos[1] > port1.pos[1]) & (playerPos[1] + playerSize[1] < port1.pos[1] + 32)) { // teleport from blue
            if ((port1.side == 0) && (playerPos[0] == port1.pos[0]) && (port2_pos[0] != -500)) {
                player_pos[0] = port2.pos[0];
                player_pos[1] = port2.pos[1] + 5;

                playerPos[0] = port2.pos[0];
                playerPos[1] = port2.pos[1] + 5;

            } else if ((port1.side == 1) && (port1.pos[0] - playerPos[0] == playerSize[0] + 5) 
            && (port2_pos[0] != -500)) {
                player_pos[0] = port2.pos[0] + 10;
                player_pos[1] = port2.pos[1] + 5;
                world_move = (port1_pos[0] - port2_pos[0]) * (port1_pos[0] * .05);

                playerPos[0] = port2.pos[0] + 10;
                playerPos[1] = port2.pos[1] + 5;
            }

        } if ((playerPos[1] > port2.pos[1]) & (playerPos[1] + playerSize[1] < port2.pos[1] + 32)) { // teleport from orange
            if ((port2.side == 0) && (playerPos[0] == port2.pos[0] && (port1_pos[0] != -500))) {
                player_pos[0] = port1.pos[0] - 10;
                player_pos[1] = port1.pos[1] + 5;
                world_move = (port1_pos[0] - port2_pos[0]) * (port2_pos[0] * .05);

                playerPos[0] = port1.pos[0] - 10;
                playerPos[1] = port1.pos[1] + 5;

            } else if ((port2.side == 1) && (playerPos[0] + playerSize[0] > port2.pos[0] + 4) 
            && (port1_pos[0] != -500)) {
                playerPos[0] = port1.pos[0] + 10;
                playerPos[1] = port1.pos[1] + 5;
            }
        }

        /*if ((playerPos[0] > 237)) { // no going through the walls (needs delete)
            playerPos[0] = 237;
            player_pos[0] += world_move;
        } else if ((playerPos[0] < 0)) {
            playerPos[0] = 0;
            player_pos[0] += world_move;
        } */

        playerPos[1] += playerSpeed; // moving
        world_move += playerHorSpeed;
        cube.pos[1] += cube.speed;

        if (KEY_R & keysHeld()) { // aiming logic 
            world_move = 0;
        }

        if ((wall2.collide_side()) && (facing == 0)) world_move = 0;
        if ((wall3.collide_side()) && (facing == 1)) world_move = 0;


        if (wall1.collide_top() == 0) { // basic falling
            playerSpeed += 0.125;

            if (KEY_R & keysHeld()) ChellFrame = 12;
            ChellFrame = 9;
        } else { // basic not falling
            playerPos[1] = wall1.pos[1] - playerSize[1];
            jumped = 0;
            playerSpeed = 0;
            playerHorSpeed = 0;

            if (KEY_R & keysHeld()) ChellFrame = 11;
        }

        if (cube.pos[1] < wall1.pos[1] - 11) {
            cube.speed += 0.125;
        } else {
            cube.speed = 0;
            isHolding = 0;
            cube.pos[1] = wall1.pos[1] - 11;
        }

        if ((player_pos[0] < -(world_size[0] / 2)) 
        || (player_pos[0] > world_size[0] / 2)) {
            playerPos[0] -= world_move;
            player_pos[0] -= world_move;
        } else {
            doorPos[0] += world_move;
            AFPlatePos[0] += world_move;
            flButtonPos[0] += world_move;
            port1.pos[0] += world_move;
            port2.pos[0] += world_move;
            cube.pos[0] += world_move;
            cDropperPos[0] += world_move;
            checkX += world_move;
            connectPos[0] += world_move;
            connectPos[2] += world_move;
            WheatleyPos[0] += world_move;
            piButtonPos[0] += world_move;
            warnX += world_move;

            wall1.pos[0] += world_move;
            wall1.pos[2] += world_move;
            wall2.pos[0] += world_move;
            wall2.pos[2] += world_move;
            wall3.pos[0] += world_move;
            wall3.pos[2] += world_move;

            /*if (KEY_R & keysHeld()) playerPos[0] += world_move;
            else if (KEY_R & keysUp()) {
                world_move = SCREEN_WIDTH / 2 - playerPos[0] - 13;
            } else {
                playerPos[0] = 115;
                player_pos[0] -= world_move;
            }*/
            playerPos[0] = 115;
            player_pos[0] -= world_move;
        }

        glEnd2D();
        glFlush(0);

        if (KEY_START & keysDown()) break;
        
        swiWaitForVBlank();
    }
}