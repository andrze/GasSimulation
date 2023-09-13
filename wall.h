#ifndef WALL_H
#define WALL_H
#include <ostream>

class Wall
{
public:
    Wall();
    Wall(double a1, double a2, double b, bool horizontal);

    double a1, a2;
    double b;
    bool horizontal;
};

bool operator == (Wall lhs, Wall rhs);
std::ostream& operator << (std::ostream& out, Wall w);

#endif // WALL_H
