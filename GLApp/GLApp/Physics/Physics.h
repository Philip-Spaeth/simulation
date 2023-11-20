#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>
#include "Particle.h"
#include <glm.hpp>
#include "Engine.h"
#include "FileManager.h"

#define TARGET_FPS 30

class SystemInit;
class FileManager;

class Physics {
public:
    Physics();

    bool Calc();

    void setRandomSeed(unsigned int seed);
    double random(double min, double max);

    static const int numTimeSteps = 1000;
    static const int particlesSize = 10;

    // one hour = 3600
    // one day = 86400
    // one year = 31536000
    // galaxy = 1e16

    //the time per frame
    const double deltaTime = 86400;


    //Physikalische Konstanten
    const double G = 6.67430e-11;
    const double softening = 1;


    std::vector<Particle> currentParticles;
private:
    std::vector<std::vector<double>> totalEnergie;
    int calulations = 0;
    SystemInit* systemInit;
    FileManager* fileManager;
    void calcTime(int index, std::chrono::steady_clock::time_point current_time);
};

#endif // PHYSICS_H