#include "planet.h"
#include <QColor>
#include <ostream>
#include <vector>

Planet::Planet()
{
}

Planet::Planet(double m, Vector2d loc, Vector2d vel)
    : mass(m)
    , location(loc)
    , velocity(vel) {

    };

double Planet::energy()
{
    return velocity * velocity * mass / 2;
}

Vector2d Planet::momentum()
{
    return velocity * mass;
}

std::ostream& operator<<(std::ostream& out, Planet p)
{
    out << "P " << ' ' << p.location << ' ' << p.velocity << ' ' << p.mass;
    return out;
}
