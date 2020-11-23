#include <iostream>
#include <vector>
#include "constants.h"

struct MotionParameters
{
    double initalForce;
    double initalVelocity;
    double finalVelocity;
    double acceleration;
    double displacement;
    double force;
    double time;
    double frictionUK;
    double frictionUS;
};

class Entity
{
    long int posX = 0;
    long int posY = 0;
    double weight = 100;

public:
    Entity(long int posX, long int posY) : posX(posX), posY(posY) {}

    void moveTo(int posX, int posY)
    {
        this->posX = posX;
        this->posY = posY;
        std::cout << "[Position Changed] "
                  << "X: " << posX << " "
                  << "Y: " << posY << std::endl;
    }
};

class Movement
{
    Entity *target;

public:
    void setTarget(Entity *target)
    {
        this->target = target;
    }

    void moveUsingForce(MotionParameters *motionParameters)
    {
    }

    void moveUsingFinalVelocity(MotionParameters *motionParameters)
    {
    }

    void moveUsingInitialVelocityAndDisplacement(MotionParameters *motionParameters)
    {
    }
};

int main()
{
    Entity *ball = new Entity(0, 0);
    Entity *cube = new Entity(0, 0);

    MotionParameters *motionParameters = new MotionParameters();
    motionParameters->force = 600;

    Movement *movement = new Movement();
    movement->setTarget(cube);
    movement->moveUsingForce(motionParameters);

    // ball->moveTo(3, 4);
    return 0;
}
