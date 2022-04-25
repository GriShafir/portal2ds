#include <nds.h>
#include <gl2d.h>

class Portal {
    public:
        int* pos = new int[2];
        int side; // 0 - left, 1 - right
};

class Player {
    public:
        int* pos = new int[2] {90, 130};
        int jump_speed = 0;
        int jumped = 0;
};

class Gun {
    public:
        int* pos = new int[2];
        int state = 0; // 0 - blue, 1 - orange
        int jump_speed = 0;

};

int main() {
    /* sprites */
    videoSetMode(MODE_5_3D);
    glScreen2D();

    Player player;
    float playerSpeed = 0;
    float jumped = 0;

    float gunPosX = player.pos[0] + 7;
    float gunPosY = player.pos[1] + 8;
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
        glBoxFilled(player.pos[0], player.pos[1], player.pos[0] + 18, player.pos[1] + 33, RGB15(218, 255, 255)); // drawing player
        glBoxFilled(gunPosX, gunPosY, gunPosX + 20, gunPosY + 10, RGB15(221, 221, 221)); // drawing portal gun

        glBoxFilled(port1.pos[0], port1.pos[1], port1.pos[0] + 5, port1.pos[1] + 43, RGB15(128, 255, 255)); // blue portal
        glBoxFilled(port2.pos[0], port2.pos[1], port2.pos[0] + 5, port2.pos[1] + 43, RGB15(255, 109, 43)); // orange portal

        if (KEY_A & keysDown() & jumped == 0) { // basic jumping
            player.jump_speed = -6;
            player.jumped = 1;

            gunJump = -6;
        } else if (KEY_LEFT & keysHeld()) { // basic walking
            player.pos[0] -= 3;

            gunPosX = player.pos[0] - 7;
            gunPosX -= 3;

            is255 = 0;
        } else if (KEY_RIGHT & keysHeld()) {
            player.pos[0] += 3;

            gunPosX = player.pos[0] + 7;
            gunPosX += 3;

            is255 = 1;
        } else if (KEY_X & keysDown()) { // creating blue portal
            port1.pos[1] = player.pos[1] - 5;

            if (port1.side == 1) {
                port1.pos[0] = 250;
            } else {
                port1.pos[0] = 0;
            }

            glBoxFilled(port1.pos[0], port1.pos[1], port1.pos[0] + 5, port1.pos[1] + 43, RGB15(128, 255, 255));

            // mmEffectEx(&fire_blue);
        } else if (KEY_Y & keysDown()) { // creating orange portal
            port2.pos[1] = player.pos[1] - 5;

            if (port2.side == 1) {
                port2.pos[0] = 250;
            } else {
                port2.pos[0] = 0;
            }

            glBoxFilled(port2.pos[0], port2.pos[1], port2.pos[0] + 5, port2.pos[1] + 43, RGB15(255, 109, 43));

            // mmEffectEx(&fire_orange);
        }

        if (player.pos[0] + 18 > port1.pos[0] + 5) { // basic blue portal behaviour
            if (player.pos[1] > port1.pos[1] & player.pos[1] + 33 < port1.pos[1] + 43) {
                player.pos[0] = port2.pos[0] + 3;
                player.pos[1] = port2.pos[1] + 5;

                gunPosX = port2.pos[0] + 7;
                gunPosY = port2.pos[1] + 13;
            }
        } else if (player.pos[0] < port2.pos[0]) { // basic orange portal behaviour
            if (port2.side == 0) {
                if (player.pos[1] > port2.pos[1] & player.pos[1] + 33 < port2.pos[1] + 43) {
                    player.pos[0] = port1.pos[0] - 3;
                    player.pos[1] = port1.pos[1] + 5;

                    gunPosX = port1.pos[0] - 10;
                    gunPosY = port1.pos[1] + 13;
                }
            } else {
                if (player.pos[1] > port2.pos[1] & player.pos[1] + 33 < port2.pos[1] + 43) {
                    player.pos[0] = port1.pos[0] + 3;
                    player.pos[1] = port1.pos[1] + 5;

                    gunPosX = port1.pos[0] + 7;
                    gunPosY = port1.pos[1] + 13;
                }
            }
        }

        if (player.pos[0] > 237) { // no going outside of the screen
            player.pos[0] = 237;
        } else if (player.pos[0] < 0) {
            player.pos[0] = 0;
        } 

        player.pos[1] += player.jump_speed;
        gunPosX += gunSpeed;
        gunPosY += gunJump;

        if (player.pos[1] < 130) { // basic falling
            player.jump_speed += 0.125;
            gunJump += 0.125;
        } else {
            player.pos[1] = 130;
            player.jumped = 0;
            gunPosY = player.pos[1] + 8;
        }

        glEnd2D();
        glFlush(0);
        
        swiWaitForVBlank();
    }
}
