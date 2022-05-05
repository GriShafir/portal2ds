#include <nds.h>
#include <stdio.h>
#include <gl2d.h>

class Portal {
    public:
        int side; // 0 - left, 1 - right
        int* pos = new int[2];
};

int main() {
    /* sprites */
    videoSetMode(MODE_5_3D);
    consoleDemoInit();
    glScreen2D();

    int playerPosX = 90;
    int playerPosY = 130;
    float playerSpeed = 0;
    int jumped = 0;
    int facing = 1; // 0 - left; 1 - right

    int gunPosX = playerPosX + 7;
    int gunPosY = playerPosY + 8;
    float gunSpeed = 0;

    float cubePosX = 125;
    float cubePosY = 145;
    float size = 18;

    Portal port1;
    Portal port2;

    port1.pos[0] = 250;
    port1.pos[1] = 125;
    port1.side = 1;

    port2.pos[0] = 0;
    port2.pos[1] = 125;
    port2.side = 0;

    // int is255 = 0;

    /* game logic */
    while (1) {
        scanKeys();

        glBegin2D();
        glBoxFilled(playerPosX, playerPosY, playerPosX + 18, playerPosY + 33, RGB15(218, 255, 255)); // drawing player
        glBoxFilled(gunPosX, gunPosY, gunPosX + 20, gunPosY + 10, RGB15(221, 221, 221)); // drawing portal gun

        // glBoxFilled(cubePosX, cubePosY, cubePosX + size, cubePosY + size, RGB15(221, 221, 221)); // drawing cube

        glBoxFilled(port1.pos[0], port1.pos[1], port1.pos[0] + 5, port1.pos[1] + 43, RGB15(128, 255, 255)); // blue portal
        glBoxFilled(port2.pos[0], port2.pos[1], port2.pos[0] + 5, port2.pos[1] + 43, RGB15(255, 109, 43)); // orange portal

        iprintf("\x1b[2J");
        iprintf("Portal2DS - Made by GriShafir\n");
        iprintf("\nExample of Portal game in 2D \nmade for Nintendo DS.\n");

        iprintf("\nTo move around, press Left and \nRight on the D-Pad.");
        iprintf("\nTo jump, press A.");
        iprintf("\nTo create blue portal, press X.");
        iprintf("\nTo create orange portal, press \nY.");

        iprintf("\nPlayer's coordinates: \n%i, %i\n", playerPosX, playerPosY);
        iprintf("\nBlue portal's coordinates: \n%i, %i\n", port1.pos[0], port1.pos[1]);
        iprintf("\nOrange portal's coordinates: \n%i, %i\n", port2.pos[0], port2.pos[1]);

        // iprintf("\nBlue portal's side: \n%i\n", port1.side);
        // iprintf("\nOrange portal's side: \n%i\n", port2.side);
        
        if ((KEY_A & keysDown()) & (jumped == 0)) { // basic jumping
            playerSpeed = -3;
            jumped = 1;
        } else if (KEY_LEFT & keysHeld()) { // basic walking
            playerPosX -= 3;

            facing = 0;
        } else if (KEY_RIGHT & keysHeld()) {
            playerPosX += 3;

            facing = 1;
        } 
        
        if (KEY_X & keysDown()) { // creating blue portal
            port1.pos[1] = playerPosY - 5;

            if (facing == 1) {
                port1.pos[0] = 250;
                port1.side = 1;
            } else {
                port1.pos[0] = 0;
                port1.side = 0;
            }
            // mmEffectEx(&fire_blue);

        } else if (KEY_Y & keysDown()) { // creating orange portal
            port2.pos[1] = playerPosY - 5;

            if (facing == 1) {
                port2.pos[0] = 250;
                port2.side = 1;
            } else {
                port2.pos[0] = 0;
                port2.side = 0;
            }
            // mmEffectEx(&fire_orange);
        }

        /* else if (KEY_R & keysDown() & weigh_cube.got == 0) { // getting cube
            weigh_cube.pos[1] = playerPosY;
            weigh_cube.speed = 54;
        } else if (KEY_R & keysDown() & weigh_cube.got == 1) { // placing cube
            weigh_cube.got = 0;
        } */

        if (port1.side == 1) {
            if (playerPosX + 18 > port1.pos[0] + 5) {
                if ((playerPosY > port1.pos[1]) & (playerPosY + 33 < port1.pos[1] + 43)) {
                    playerPosX = port2.pos[0] + 10;
                    playerPosY = port2.pos[1] + 5;

                    gunPosX = port2.pos[0] + 7;
                    gunPosY = port2.pos[1] + 13;
                }
            }
        } else {
            if (playerPosX < port1.pos[0]) {
                if ((playerPosY > port1.pos[1]) & (playerPosY + 33 < port1.pos[1] + 43)) {
                    playerPosX = port2.pos[0] - 10;
                    playerPosY = port2.pos[1] + 5;

                    gunPosX = port2.pos[0] - 10;
                    gunPosY = port2.pos[1] + 13;
                }
            }
        }

        if (port2.side == 0) {
            if (playerPosX < port2.pos[0]) {
                if ((playerPosY > port2.pos[1]) & (playerPosY + 33 < port2.pos[1] + 43)) {
                    playerPosX = port1.pos[0] - 10;
                    playerPosY = port1.pos[1] + 5;

                    gunPosX = port1.pos[0] - 10;
                    gunPosY = port1.pos[1] + 13;
                }
            }
        } else {
            if (playerPosX + 18 > port2.pos[0] + 5) {
                if ((playerPosY > port2.pos[1]) & (playerPosY + 33 < port2.pos[1] + 43)) {
                    playerPosX = port1.pos[0] + 10;
                    playerPosY = port1.pos[1] + 5;

                    gunPosX = port1.pos[0] + 7;
                    gunPosY = port1.pos[1] + 13;
                }
            }
        }

        /* if (playerPosX + 18 > port1.pos[0] + 5) { // basic blue portal behaviour
            if (port1.side == 1) {
                if (playerPosY > port1.pos[1] & playerPosY + 33 < port1.pos[1] + 43) {
                    playerPosX = port2.pos[0] + 3;
                    playerPosY = port2.pos[1] + 5;

                    gunPosX = port2.pos[0] + 7;
                    gunPosY = port2.pos[1] + 13;

                    /* cubePosX = port2.pos[0] + 54;
                    cubePosY = port2.pos[1] + 3; 
                }
            } else {
                if (playerPosY > port1.pos[1] & playerPosY + 33 < port1.pos[1] + 43) {
                    playerPosX = port2.pos[0] - 3;
                    playerPosY = port2.pos[1] + 5;

                    gunPosX = port2.pos[0] + 7;
                    gunPosY = port2.pos[1] + 13;
                }
            }
        }  else if (playerPosX < port2.pos[0]) { // basic orange portal behaviour
            if (port2.side == 0) {
                if (playerPosY > port2.pos[1] & playerPosY + 33 < port2.pos[1] + 43) {
                    playerPosX = port1.pos[0] - 3;
                    playerPosY = port1.pos[1] + 5;

                    gunPosX = port1.pos[0] - 10;
                    gunPosY = port1.pos[1] + 13;
                }
            } else {
                if (playerPosY > port2.pos[1] & playerPosY + 33 < port2.pos[1] + 43) {
                    playerPosX = port1.pos[0] + 3;
                    playerPosY = port1.pos[1] + 5;

                    gunPosX = port1.pos[0] + 7;
                    gunPosY = port1.pos[1] + 13;
                }
            }
        } */

        if (facing == 0) { // gun's rendering
            gunPosX = playerPosX - 7;
            gunPosX -= 3;
        } else if (facing == 1) {
            gunPosX = playerPosX + 7;
            gunPosX += 3;
        }

         if (playerPosX > 237) { // no going through the walls
            playerPosX = 237;
        } else if (playerPosX < 0) {
            playerPosX = 0;
        } 

        playerPosY += playerSpeed;
        gunPosX += gunSpeed;
        gunPosY += playerSpeed;
        // cubePosX = cubeSpeed;

        if (playerPosY < 130) { // basic falling
            playerSpeed += 0.125;
        } else {
            playerPosY = 130;
            jumped = 0;
            gunPosY = playerPosY + 8;
        }

        glEnd2D();
        glFlush(0);
        
        swiWaitForVBlank();
    }
}

