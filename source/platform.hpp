// this is platforms, collisions etc.
class Platform
{
public:
    int type;               // 0 - non-portable, 1 - portable
    std::array<int, 4> pos; // {x1, y1, x2, y2}

    int collide_top(int *playerPos, int *playerSize) const
    {
        Platform g;
        g = *this;

        if ((playerPos[1] + playerSize[1] > g.pos[1]) && (playerPos[1] < g.pos[3]) &&
            (playerPos[0] + playerSize[0]/4 < g.pos[2]) && (playerPos[0] + playerSize[0]/4*3 > g.pos[0]))
            return 1;
        else
            return 0;
    }

    int collide_side(int *playerPos, int *playerSize) const
    {
        Platform g;
        g = *this;

        if ((playerPos[0] < g.pos[2]) &&
            (playerPos[0] + playerSize[0] > g.pos[0]) &&
            (playerPos[1] + playerSize[1] - 2 > g.pos[1]) &&
            (playerPos[1] < g.pos[3])) {
                if (playerPos[0] + playerSize[0] < g.pos[2]) {
                    return 1;
                } else if (playerPos[0] > g.pos[0]) {
                    return 2;
                } else return 0;
            }
        else
            return 0;
    }

    void draw() const
    {
        Platform g;
        g = *this;
        int color = RGB15(0, 0, 0);
        if (g.type == 0)
            color = RGB15(5, 5, 5);
        else if (g.type == 1)
            color = RGB15(219, 219, 219);
        glBoxFilled(g.pos[0], g.pos[1], g.pos[2], g.pos[3], color);
    }
};

class Cube
{
public:
    int type;               // 0 - weighted, 1 - companion, 2 - laser, 3 - contraption
    std::array<int, 2> pos; // {x1, y1}

    float speed;
};