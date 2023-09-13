#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <ostream>

class Vector2d
{
public:
    Vector2d();
    Vector2d(double x, double y);

    double x;
    double y;

    Vector2d& operator += (Vector2d rhs);
    Vector2d& operator *= (double rhs);
    Vector2d& operator -= (Vector2d rhs);
    Vector2d& operator /= (double rhs);
    double length();
};

bool operator == (Vector2d lhs, Vector2d rhs);
Vector2d operator + (Vector2d lhs, Vector2d rhs);
Vector2d operator - (Vector2d lhs, Vector2d rhs);
Vector2d operator * (Vector2d lhs, double rhs);
Vector2d operator * (double lhs, Vector2d rhs);
Vector2d operator / (Vector2d lhs, double rhs);
double operator * (Vector2d lhs, Vector2d rhs);
std::ostream& operator << (std::ostream& out, Vector2d v);

#endif // VECTOR2D_H
