#ifndef DATA_H
#define DATA_H
#include <string>
#include <vector>
#include <fstream>
#include <QLine>
#include "planet.h"
#include "vector2d.h"
#include "wall.h"

class Data
{
public:
    Data();
    Data(std::ifstream& stream);
    Data(std::vector<Planet> planets,std::vector<Wall> walls={});
    Data(size_t num_planets, size_t num_walls);

    std::vector<Planet> planets;
    std::vector<Wall> walls;
    double step;
    double gravity=0;

    double temperature();
    std::vector<Vector2d> acceleration();
    std::string statistics();
    void move(double temperature);
    void set_temperature(double temp);
};

std::ostream& operator<<(std::ostream& out, Data data);

#endif // DATA_H
