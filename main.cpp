#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include "constants.h"
#include <chrono>

using namespace std::chrono;

struct MotionParameter
{
    double X;
    double Y;
    double Z;

    MotionParameter(double X, double Y, double Z) : X(X), Y(Y), Z(Z){};
};

struct MotionParameters
{
    MotionParameter *initalForce;
    MotionParameter *initialVelocity;
    MotionParameter *finalVelocity;
    MotionParameter *acceleration;
    MotionParameter *displacement;
    double time;
    double frictionUK;
    double frictionUS;
};

class Entity
{
    double posX = 0;
    double posY = 0;
    double posZ = 0;

public:
    double mass = 0;

    Entity(double posX, double posY, double posZ, double mass)
        : posX(posX), posY(posY), posZ(posZ), mass(mass) {}

    void moveTo(MotionParameter *displacement)
    {
        this->posX = displacement->X;
        this->posY = displacement->Y;
        this->posZ = displacement->Z;
        std::cout << "[Position Changed] "
                  << "X: " << posX << " "
                  << "Y: " << posY << " "
                  << "Z: " << posZ << std::endl;
    }
};

class Movement
{
    Entity *target;

private:
    double calculateTimeByAUS(double a, double u, double s)
    {
        double finalVelocity = sqrt(pow(u, 2) + 2 * a * s);
        if (std::isnan(finalVelocity))
            throw "Time Cannot Be Calculated!";

        double time = (finalVelocity - u) / a;
        if (std::isnan(time))
            return 0;
        return time;
    }

    double calculateDisplacementByAUT(double a, double u, double t)
    {
        double displacement = u * t + 1 / 2 * a * pow(t, 2);
        return displacement;
    }

    void sleep(int milli)
    {
        // Cross-platform sleep function
        clock_t end_time;
        end_time = clock() + milli * CLOCKS_PER_SEC / 1000;
        while (clock() < end_time)
        {
        }
    }

public:
    void setTarget(Entity *target)
    {
        this->target = target;
    }

    void moveUsingForceAndInitialVelocity(MotionParameters *motionParameters)
    {
        // Acceleration
        double netAccX = (motionParameters->initalForce->X - motionParameters->frictionUK * this->target->mass * aGravity) / this->target->mass;
        double netAccY = (motionParameters->initalForce->Y - motionParameters->frictionUK * this->target->mass * aGravity) / this->target->mass;
        double netAccZ = (motionParameters->initalForce->Z - motionParameters->frictionUK * this->target->mass * aGravity) / this->target->mass;

        if (netAccX < 0 && motionParameters->initialVelocity->X <= 0)
            netAccX = 0;
        if (netAccY < 0 && motionParameters->initialVelocity->Y <= 0)
            netAccY = 0;
        if (netAccZ < 0 && motionParameters->initialVelocity->Z <= 0)
            netAccZ = 0;

        // Displacement
        double netDisX = motionParameters->displacement->X;
        double netDisY = motionParameters->displacement->Y;
        double netDisZ = motionParameters->displacement->Z;

        double timeX = calculateTimeByAUS(netAccX, motionParameters->initialVelocity->X, netDisX);
        double timeY = calculateTimeByAUS(netAccY, motionParameters->initialVelocity->Y, netDisY);
        double timeZ = calculateTimeByAUS(netAccZ, motionParameters->initialVelocity->Z, netDisZ);

        auto start = high_resolution_clock::now();
        //  Varying Time
        for (double i, j, k = 0;
             //  i < timeX, j < timeY, k < timeZ;
             //  i += milliSecondsPerFPS, j += milliSecondsPerFPS, k += milliSecondsPerFPS)
             i < timeX;
             i += secondsPerFPS)
        {
            double disX = calculateDisplacementByAUT(netAccX, motionParameters->initialVelocity->X, i);
            // double disY = calculateDisplacementByAUT(netAccY, motionParameters->initialVelocity->Y, timeY);
            // double disZ = calculateDisplacementByAUT(netAccZ, motionParameters->initialVelocity->Z, timeZ);
            // MotionParameter *displacement = new MotionParameter(disX, disY, disZ);
            MotionParameter *displacement = new MotionParameter(disX, 0, 0);
            this->target->moveTo(displacement);

            sleep(milliSecondsPerFPS);
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << duration.count() << std::endl;
        char a;
        std::cin >> a; 
    }

    void moveUsingFinalVelocity(MotionParameters *motionParameters)
    {
    }

    void moveUsingDisplacement(MotionParameters *motionParameters)
    {
    }
};

int main()
{
    Entity *ball = new Entity(0, 0, 0, 15);
    Entity *cube = new Entity(0, 0, 0, 15);

    // Define motion
    MotionParameters *motionParameters = new MotionParameters();

    // Motion Parameters
    motionParameters->frictionUK = .4;
    MotionParameter *initialForce = new MotionParameter(70, 0, 0);
    motionParameters->initalForce = initialForce;
    MotionParameter *displacement = new MotionParameter(20, 0, 0);
    motionParameters->displacement = displacement;
    MotionParameter *initialVelocity = new MotionParameter(15, 0, 0);
    motionParameters->initialVelocity = initialVelocity;

    // Create Movement Specification
    Movement *movement = new Movement();
    movement->setTarget(cube);

    // Move the Enitity
    movement->moveUsingForceAndInitialVelocity(motionParameters);

    return 0;
}
