#include <nds.h>
#include <gl2d.h>

class Portal {
    public:
        int side; // 0 - left, 1 - right
        int* pos = new int[2];
};

int main() {
    /* sprites */
    videoSetMode(MODE_5_3D);
    glScreen2D();

    float playerPosX = 90;
    float playerPosY = 130;
    float playerSpeed = 0;
    float jumped = 0;

    float gunPosX = playerPosX + 7;
    float gunPosY = playerPosY + 8;
    float gunSpeed = 0;
    float gunJump = 0;

    Portal port1;
    Portal port2;

    port1.pos[0] = 250;
    port1.pos[1] = 125;
    port1.side = 1;

    port2.pos[0] = 0;
    port2.pos[1] = 125;
    port2.side = 0;

    int is255 = 0;

    /* game logic */
    while (1) {
        scanKeys();

        glBegin2D();
        glBoxFilled(playerPosX, playerPosY, playerPosX + 18, playerPosY + 33, RGB15(218, 255, 255)); // drawing player
        glBoxFilled(gunPosX, gunPosY, gunPosX + 20, gunPosY + 10, RGB15(221, 221, 221)); // drawing portal gun

        glBoxFilled(port1.pos[0], port1.pos[1], port1.pos[0] + 5, port1.pos[1] + 43, RGB15(128, 255, 255)); // blue portal
        glBoxFilled(port2.pos[0], port2.pos[1], port2.pos[0] + 5, port2.pos[1] + 43, RGB15(255, 109, 43)); // orange portal

        if (KEY_A & keysDown() & jumped == 0) { // basic jumping
            playerSpeed = -6;
            jumped = 1;

            gunJump = -6;
        } else if (KEY_LEFT & keysHeld()) { // basic walking
            playerPosX -= 3;

            gunPosX = playerPosX - 7;
            gunPosX -= 3;

            is255 = 0;
        } else if (KEY_RIGHT & keysHeld()) {
            playerPosX += 3;

            gunPosX = playerPosX + 7;
            gunPosX += 3;

            is255 = 1;
        } else if (KEY_X & keysDown()) { // creating blue portal

            port1.pos[1] = playerPosY - 5;

            if (port1.side == 1) {
                port1.pos[0] = 250;
            } else {
                port1.pos[0] = 0;
            }

            glBoxFilled(port1.pos[0], port1.pos[1], port1.pos[0] + 5, port1.pos[1] + 43, RGB15(128, 255, 255));

            // mmEffectEx(&fire_blue);
        } else if (KEY_Y & keysDown()) { // creating orange portal


            port2.pos[1] = playerPosY - 5;

            if (port2.side == 1) {
                port2.pos[0] = 250;
            } else {
                port2.pos[0] = 0;
            }

            glBoxFilled(port2.pos[0], port2.pos[1], port2.pos[0] + 5, port2.pos[1] + 43, RGB15(255, 109, 43));

            // mmEffectEx(&fire_orange);
        }

        if (playerPosX + 18 > port1.pos[0] + 5) { // basic blue portal behaviour
            if (playerPosY > port1.pos[1] & playerPosY + 33 < port1.pos[1] + 43) {
                playerPosX = port2.pos[0] + 3;
                playerPosY = port2.pos[1] + 5;

                gunPosX = port2.pos[0] + 7;
                gunPosY = port2.pos[1] + 13;
            }
        } else if (playerPosX < port2.pos[0]) { // basic orange portal behaviour
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
        }

        if (playerPosX > 237) {
            playerPosX = 237;
        } else if (playerPosX < 0) {
            playerPosX = 0;
        } 

        playerPosY += playerSpeed;
        gunPosX += gunSpeed;
        gunPosY += gunJump;

        if (playerPosY < 130) { // basic falling
            playerSpeed += 0.125;
            gunJump += 0.125;
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
