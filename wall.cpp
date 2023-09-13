#include "wall.h"

Wall::Wall()
{

}

Wall::Wall(double a1, double a2, double b, bool horizontal){
    this->a1 = a1;
    this->a2 = a2;
    this->b = b;
    this->horizontal = horizontal;
}

bool operator == (Wall lhs, Wall rhs){
    return (lhs.a1==rhs.a1 && lhs.a2==rhs.a2 && lhs.b==rhs.b && lhs.horizontal==rhs.horizontal);
}

std::ostream& operator<<(std::ostream& out, Wall w) {
    out << "W ";
    if(w.horizontal){
        out << "H ";
    } else {
        out << "V ";
    }
    out << w.a1 << ' ' << w.a2 << ' ' << w.b;
    return out;
}
