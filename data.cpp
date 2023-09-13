#include "data.h"
#include "planet.h"
#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Data::Data()
{
    walls = { { -2.5, 2.5, -2.5, false }, { -2.5, 2.5, -2.5, true }, { -2.5, 2.5, 2.5, false }, { -2.5, 2.5, 2.5, true } };
}

Data::Data(std::ifstream& stream)
{
    step = 0.0001;
    std::string line;
    while (getline(stream, line)) {
        std::istringstream sstream(line);
        char type;
        sstream >> type;
        if (type == 'W') {
            char wall_type;
            sstream >> wall_type;
            Wall wall;
            sstream >> wall.a1 >> wall.a2 >> wall.b;
            if (wall_type == 'H') {
                wall.horizontal = true;
            } else {
                wall.horizontal = false;
            }
            bool duplicate = false;
            for (auto&& w : walls) {
                if (w == wall) {
                    duplicate = true;
                }
            }
            if (!duplicate) {
                walls.push_back(wall);
            }
            if (!sstream) {
                throw std::invalid_argument("Invalid file format");
            }
        } else {
            Planet planet;
            sstream >> planet.location.x >> planet.location.y;
            sstream >> planet.velocity.x >> planet.velocity.y;
            sstream >> planet.mass;
            planets.push_back(planet);
            if (!sstream) {
                throw std::invalid_argument("Invalid file format");
            }
        }
    }
}

Data::Data(std::vector<Planet> planets, std::vector<Wall> walls)
    : Data()
{
    step = 0.0001;
    this->planets = planets;
    if (!walls.empty()) {
        this->walls = walls;
    }
}

Data::Data(size_t num_planets, size_t num_walls)
    : Data()
{
    step = 0.0001;
    std::random_device rd {};
    std::mt19937 gen { rd() };
    std::normal_distribution<> space_dist { 0, 1 };
    std::normal_distribution<> mass_dist { 1, .5 };
    std::uniform_real_distribution<> wall_dist { -2.5, 2.5 };
    std::uniform_int_distribution<> direction_dist { 0, 1 };
    this->planets.clear();
    for (size_t i = 0; planets.size() < num_planets; i++) {
        double mass = mass_dist(gen);
        double x_coord = space_dist(gen);
        double y_coord = space_dist(gen);
        double x_vel = space_dist(gen);
        double y_vel = space_dist(gen);
        if (mass < 0 || std::abs(x_coord) > 2.5 || std::abs(x_coord) > 2.5 || std::abs(y_coord) > 2.5 || std::abs(x_vel) > 2.5 || std::abs(x_vel) > 2.5) {
            continue;
        }
        this->planets.push_back(Planet(mass, { x_coord, y_coord }, 8 * Vector2d(x_vel, y_vel)));
    }

    walls = { { -2.5, 2.5, -2.5, false }, { -2.5, 2.5, -2.5, true }, { -2.5, 2.5, 2.5, false }, { -2.5, 2.5, 2.5, true } };

    for (size_t i = 0; i < num_walls; i++) {

        bool is_horizontal = bool(direction_dist(gen));
        double a1 = wall_dist(gen);
        double a2 = wall_dist(gen);
        double b = wall_dist(gen);
        if (a1 > a2) {
            std::swap(a1, a2);
        }
        this->walls.push_back(Wall(a1, a2, b, is_horizontal));
    }
}

double Data::temperature()
{
    double energy = 0;
    for (auto&& p : planets) {
        energy += p.velocity * p.velocity * p.mass / 2;
    }
    return energy / planets.size();
}

std::vector<Vector2d> Data::acceleration()
{
    size_t n_planets = planets.size();
    Vector2d vector0(0., 0.);
    std::vector<Vector2d> acceleration_(n_planets, vector0);
    for (size_t i = 0; i < n_planets; i++) {
        for (size_t j = i + 1; j < n_planets; j++) {
            Vector2d distance = planets[i].location - planets[j].location;
            auto distance2 = distance * distance;
            auto acc = Vector2d();
            if (distance2 < 0.8) {
                auto distance6 = distance2 * distance2 * distance2 * 5000;
                acc = 0.1 * distance / sqrt(distance2) / (distance6 * distance6 - distance6);
            } else {
                acc.x = 0;
                acc.y = 0;
            }
            acceleration_[i] -= 1 / planets[i].mass * acc;
            acceleration_[j] += 1 / planets[j].mass * acc;
        }
        acceleration_[i].y += gravity * 0.001;
    }
    return acceleration_;
}

std::string Data::statistics()
{
    std::string statistics = "Id;Momentum x;Momentum y;Energy \n";
    for (size_t i = 0; i < planets.size(); i++) {
        Planet p = planets[i];
        Vector2d mom = p.momentum();
        statistics += std::to_string(i) + ";" + std::to_string(mom.x) + ";" + std::to_string(mom.y) + ";" + std::to_string(p.energy()) + "\n";
    }
    return statistics;
}

void Data::move(double temperature)
{
    size_t n_planets = planets.size();
    std::array<std::vector<Vector2d>, 4> kr, kv;
    Data temp_data = *this;
    for (int j = 0; j < 4; j++) {
        kv[j] = temp_data.acceleration();
        for (size_t i = 0; i < n_planets; i++) {
            kr[j].push_back(temp_data.planets[i].velocity);
            if (j < 2) {
                temp_data.planets[i].location = planets[i].location + step * 0.5 * kr[j][i];
                temp_data.planets[i].velocity = planets[i].velocity + step * 0.5 * kv[j][i];
            } else if (j == 2) {
                temp_data.planets[i].location = planets[i].location + step * kr[j][i];
                temp_data.planets[i].velocity = planets[i].velocity + step * kv[j][i];
            } else {
                planets[i].location += step / 6 * (kr[0][i] + 2 * kr[1][i] + 2 * kr[2][i] + kr[3][i]);
                planets[i].velocity += step / 6 * (kv[0][i] + 2 * kv[1][i] + 2 * kv[2][i] + kv[3][i]);
            }
        }
    }

    set_temperature(temperature);

    for (size_t j = 0; j < planets.size(); j++) {
        for (auto&& w : walls) {
            Vector2d loc = planets[j].location;
            if (w.horizontal) {
                if (loc.x > w.a1 && loc.x < w.a2 && std::abs(loc.y - w.b) < 0.05 && std::signbit(loc.y - w.b) != std::signbit(planets[j].velocity.y)) {
                    planets[j].velocity.y *= -1;
                }
            } else {
                if (loc.y > w.a1 && loc.y < w.a2 && std::abs(loc.x - w.b) < 0.05 && std::signbit(loc.x - w.b) != std::signbit(planets[j].velocity.x)) {
                    planets[j].velocity.x *= -1;
                }
            }
        }
    }
}

void Data::set_temperature(double temp)
{
    double old_temp = this->temperature();
    double scale = sqrt(temp / old_temp);
    for (auto&& p : planets) {
        p.velocity *= scale;
    }
}

std::ostream& operator<<(std::ostream& out, Data data)
{
    for (auto&& planet : data.planets) {
        out << planet << std::endl;
    }
    for (auto&& wall : data.walls) {
        out << wall << std::endl;
    }
    return out;
}
