// this is portal-related stuff like teleport, portal placement etc.
class Portal
{
public:
    int side;               // 0 - left, 1 - right
    std::array<int, 2> pos; // {x1, y1}
    int isShown;
    int flip;


    int checkFlip() const 
    {
        Portal port;
        port = *this;
        switch (port.side) {
            case 1:
                return GL_FLIP_H; // facing left
            default:
                return GL_FLIP_NONE; // facing right
        }
    }

    int collide_platform(Platform g) const 
    {
        Portal port;
        port = *this;

        if ((port.pos[0] < g.pos[2]) &&
            (port.pos[0] + 3 > g.pos[0]) &&
            (port.pos[1] + 29 > g.pos[1]) &&
            (port.pos[1] < g.pos[3])) return 1;
        else
            return 0;
    }

    void shoot(Platform plat, int facing, int *playerPos) {
        Portal port;
        port = *this;
        port.isShown = 0;

        switch (facing) {
            case 1:
                port.pos[0] = playerPos[0] + 30;
                port.flip = GL_FLIP_H;
                break;
            default:
                port.pos[0] = playerPos[0] - 5;
                port.flip = GL_FLIP_NONE;
                break;
        }

        if (port.side == 1) {
            for (int i = 0; i < 255; i++) { 
                if (port.collide_platform(plat) == 1) port.isShown = 1;
                else port.pos[0] += 1;  
            }
        }
        else {
            for (int i = 0; i < 255; i++) { 
                if (port.collide_platform(plat) == 1) port.isShown = 1;
                else port.pos[0] -= 1;
            }
        }
    }

    int collide(int *playerPos, int *playerSize) const
    {
        Portal port;
        port = *this;

        if ((playerPos[0] < port.pos[0] + 3) &&
            (playerPos[0] + playerSize[0] > port.pos[0]) &&
            (playerPos[1] + playerSize[1] > port.pos[1]) &&
            (playerPos[1] < port.pos[1] + 29) &&
            (playerPos[1] > port.pos[1])) return 1;
        else return 0;
    }

    int teleport(int *playerPos, int *playerSize, int facing, Portal port2) const 
    {
        Portal port1 = *this;
        float motion = 0;

        if (port1.side != port2.side) 
        {
            if ((port1.collide(playerPos, playerSize) == 1) && (facing == port1.side)) {
                    if (port1.pos[0] < port2.pos[0]) motion = -abs(port2.pos[0] - port1.pos[0] - 25);
                    if (port1.pos[0] > port2.pos[0]) motion = abs(port2.pos[0] - port1.pos[0] + 25);
            }
        } else if (port1.side == port2.side) 
        {
            if ((port1.collide(playerPos, playerSize)) && (facing == port1.side)) {
                    if (port1.pos[0] < port2.pos[0]) motion = -abs(port1.pos[0] - port2.pos[0]) + 25;
                    if (port1.pos[0] > port2.pos[0]) motion = abs(port1.pos[0] - port2.pos[0]) + 25;
            }
        }
        playerPos[1] = port2.pos[1] + 5;
        return motion;
    }

    int animate(int Frame, int PortalFrame) const 
    {
        if ((Frame & 5) == 0)
        { // portal animated
            PortalFrame++;

            if (PortalFrame > 5)
            {
                PortalFrame = 0;
            }
        }
        return PortalFrame;
    }
};