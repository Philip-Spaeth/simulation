#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>
#include "Particle.h"
#include <glm.hpp>
#include "BarnesHut/Octree.h"
#include "FileManager.h"

#define TARGET_FPS 30

class SystemInit;
class FileManager;
class Octree;

class Physics {
public:
    Physics();

    bool Calc();

    void setRandomSeed(unsigned int seed);
    double random(double min, double max);

    static const bool SPH = true;
    static const bool PlummerSoftening = false;

    static const bool darkEnergy = false;
    static const int HubbleConstant = 70;

    static const int numTimeSteps = 1000;
    static const int particlesSize = 1000;

    // calculation Method:
    int calculationMethod = 0;
    
    // one hour = 3600
    // one day = 8ss6400
    // one year = 31536000
    // big galaxy = 1e16
    // galaxy = 1e13

    //the time per frame
    const double deltaTime = 1e13;

    //barnes hut
    const double theta = 1;
    const double maxDepth = 30;
    //Color 
    static const bool color = false;
    static const int colorDepth = 5;
    const double minMass = 0;
    const double maxMass = 1e38;
    
    //Plummer Softening 
    const double softening = 1e18;
    const double a = 1e100;


    double maxDistance = 0;

    //Physikalische Konstanten
    const double G = 6.67430e-11;


    std::vector<Particle> currentParticles;
private:
    Octree* octree;
    std::vector<std::vector<double>> totalEnergie;
    double calulations = 0;
    SystemInit* systemInit;
    FileManager* fileManager;
    void calcTime(int index, std::chrono::system_clock::time_point current_time);
    std::chrono::system_clock::time_point time;
    void calculateGravitation(int timeStap);
    void calculateGravitation(int timeStap, int start, int stop);
};

#endif // PHYSICS_H