#ifndef PLANET_H
#define PLANET_H
#include "vector2d.h"
#include <QColor>


struct Planet{
    Planet();
    Planet(double m, Vector2d loc, Vector2d vel);

    double mass;
    Vector2d location;
    Vector2d velocity;

    double energy();
    Vector2d momentum();
};


std::ostream& operator << (std::ostream& out, Planet p);

#endif // PLANET_H
