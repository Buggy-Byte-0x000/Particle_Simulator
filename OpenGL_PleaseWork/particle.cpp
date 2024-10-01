#include <iostream>
#include <array>
using namespace std;

class Particle {
public:
    float xCoord;
    float yCoord;
    float radius;
    float xVel;
    float yVel;
    unsigned int direction;
    bool cursorCollisionCooldown;
    array<float, 2> lastTickCoords;

    Particle(float x, float y, float r) {
        xCoord = x;
        yCoord = y;
        radius = r;
        xVel = 0;
        yVel = 0;
        direction = 270;
        cursorCollisionCooldown = false;
        lastTickCoords[0] = x;
        lastTickCoords[1] = y;
    }

    float get_xCoord() const {
        return xCoord;
    }
    float get_yCoord() const {
        return yCoord;
    }
    float getRadius() const {
        return radius;
    }
    float get_xVelocity() const {
        return xVel;
    }
    float get_yVelocity() const {
        return yVel;
    }
    unsigned int getDirection() const {
        return direction;
    }
    bool hasCursorCollisionCooldown() const {
        return cursorCollisionCooldown;
    }
    array<float, 2> getLastTickCoords() const {
        return lastTickCoords;
    }

    void set_xCoord(float new_xCoord) {
        if (abs(xCoord) < 1 - radius) {
            lastTickCoords[0] = xCoord;
        }
        xCoord = new_xCoord;
    }
    void set_yCoord(float new_yCoord) {
        if (abs(yCoord) < 1 - radius) {
            lastTickCoords[1] = yCoord;
        }
        yCoord = new_yCoord;
    }
    void set_xVelocity(float newVelocity) {
        xVel = newVelocity;
    }
    void set_yVelocity(float newVelocity) {
        yVel = newVelocity;
    }
    void setDirection(unsigned int newDirection) {
        direction = newDirection;
    }
    void enableCursorCollisionCooldown() {
        cursorCollisionCooldown = true;
    }
    void disableCursorCollisionCooldown() {
        cursorCollisionCooldown = false;
    }
};
