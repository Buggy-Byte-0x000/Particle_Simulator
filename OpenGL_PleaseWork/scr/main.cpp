#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "../particle.cpp"
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <random>
#include <cmath>

unsigned int TICK_RATE = 10000;
float FRAME_TIME = 1000 / TICK_RATE;
float G = 4;
float FRICTION_RATE = 0.9;
float CURSOR_SIZE = 0.01;

//ASK TEACHER WHY THE DIFFERENCE IN SPEED BETWEEN TICK RATE 1000 AND 1001 and how to achieve equal real time execution




void cursorCollision(Particle& cursor, Particle& particle) {
    //(convert vector to angle in radians)
    float angle = (atan(cursor.get_xCoord() / cursor.get_yCoord()))/180;
    float xVel = particle.get_xVelocity();
    float yVel = particle.get_yVelocity();

    //(unit vector normal to the wall/line of collision)
    float normalVector[2];
    normalVector[0] = cos(angle);
    normalVector[1] = sin(angle);
    //(unit vector parallel to the wall/line of collision)
    float tangentVector[2];
    tangentVector[0] = -sin(angle);
    tangentVector[1] = cos(angle);

    float normalVelocity = xVel*cos(angle) + yVel*sin(angle);
    float tangentVelocity = xVel*-sin(angle) + yVel*cos(angle);
    //(after collision)
    normalVelocity = -normalVelocity;

    float velocityUnformatted[2];
    velocityUnformatted[0] = normalVelocity*normalVector[0] + tangentVelocity*tangentVector[0];
    velocityUnformatted[1] = normalVelocity*normalVector[1] + tangentVelocity*tangentVector[1];
    //printf("%g %g \n", velocityUnformatted[0], velocityUnformatted[1]);
    particle.set_xVelocity(velocityUnformatted[0]);
    particle.set_yVelocity(velocityUnformatted[1]);
}

void borderCollision(Particle& particle) {
    //(convert vector to angle in radians)
    float x = particle.get_xCoord();
    float y = particle.get_yCoord();
    float angle = 0;
    if (x > 1)
        angle = 90;
    else if (x < -1)
        angle = 90;
    else if (y > 0.85)
        angle = 0;
    else if (y < -0.85)
        return;
    //angle = 0;
    angle = angle / 180;
    //angle = (atan(particle.get_yCoord() / particle.get_xCoord())) / 180;
    float xVel = particle.get_xVelocity();
    float yVel = particle.get_yVelocity();

    //(unit vector normal to the wall/line of collision)
    float normalVector[2];
    normalVector[0] = cos(angle);
    normalVector[1] = sin(angle);
    //(unit vector parallel to the wall/line of collision)
    float tangentVector[2];
    tangentVector[0] = -sin(angle);
    tangentVector[1] = cos(angle);

    float normalVelocity = xVel * cos(angle) + yVel * sin(angle);
    float tangentVelocity = xVel * -sin(angle) + yVel * cos(angle);
    //(after collision)
    normalVelocity = -normalVelocity;

    float velocityUnformatted[2];
    velocityUnformatted[0] = normalVelocity * normalVector[0] + tangentVelocity * tangentVector[0];
    velocityUnformatted[1] = normalVelocity * normalVector[1] + tangentVelocity * tangentVector[1];
    
    particle.set_xVelocity(velocityUnformatted[0] * FRICTION_RATE);
    particle.set_yVelocity(velocityUnformatted[1] * FRICTION_RATE);
    printf("%g \n", velocityUnformatted[1]);
    array<float, 2> lastTickCoords = particle.getLastTickCoords();
    particle.set_xCoord(lastTickCoords[0]);
    particle.set_yCoord(lastTickCoords[1]);
}

void gravityUpdate(Particle& particle) {
    float updated_velocity = particle.get_yVelocity();
    updated_velocity = updated_velocity - G/TICK_RATE;
    particle.set_yVelocity(updated_velocity);
}

void movementUpdate(Particle& particle) {
    float xCoord = particle.get_xCoord();
    float yCoord = particle.get_yCoord();
    float excess = 0;
    if (yCoord < -0.85) {
        excess = -0.85 - yCoord;
        yCoord = -0.8;
        particle.set_yVelocity(-FRICTION_RATE * particle.get_yVelocity());
    }
    xCoord = xCoord + excess + particle.get_xVelocity() * 0.5;
    yCoord = yCoord + excess + particle.get_yVelocity() * 0.005;
    particle.set_xCoord(xCoord);
    particle.set_yCoord(yCoord);
}

bool hasCollision(Particle particle1, Particle particle2) {
    bool intersection = false;
    float distance = sqrt(pow(particle2.get_xCoord() - particle1.get_xCoord(), 2) + pow(particle2.get_yCoord() - particle1.get_yCoord(), 2));
    if (distance <= particle2.getRadius()) {
        intersection = true;
    }
    return intersection;
}

bool hasBorderCollision(Particle particle) {
    float x = abs(particle.get_xCoord());
    float y = abs(particle.get_yCoord());
    float radius = particle.getRadius();
    if (x+radius >1 || y+radius > 1)
        return true;
    return false;
}

void drawCircle(GLfloat xCoord, GLfloat yCoord, GLfloat radius, bool cursorFlag) {

    unsigned int polyCount = 2000;
    GLfloat r, g, b;
    if (cursorFlag) {
        r = 1;
        g = 0;
        b = 0;
    }
    else {
        r = 1;
        g = 1;
        b = 1;
    }

    glBegin(GL_POLYGON);
    for (unsigned int i = 0; i < polyCount; i++) {
        glColor3f(r, g, b); glVertex2f(xCoord + (radius * sin(i * 720 / polyCount)), yCoord + (radius * cos(i * 720 / polyCount)));
    }
    glEnd();

    glFlush();
}

void updateCursorCoords(GLFWwindow* window, Particle &cursor) {
    double xCoord, yCoord;
    glfwGetCursorPos(window, &xCoord, &yCoord);
    //(formatting cursor coords [0,720] into openGl coords [-1,1]
    xCoord = (xCoord / 360) - 1;
    yCoord = -1 * ((yCoord / 360) - 1);
    cursor.set_xCoord(xCoord);
    cursor.set_yCoord(yCoord);
}





int main(void)
{
    //(standard GLFW window initiation)
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(720, 720, "Particle_Simulator", NULL, NULL);
    glfwSetWindowPos(window, 750, 200);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //(create a particle copying the mouse cursor movement)
    double cursor_xCoord, cursor_yCoord;
    glfwGetCursorPos(window, &cursor_xCoord, &cursor_yCoord);
    Particle cursor(cursor_xCoord, cursor_yCoord, CURSOR_SIZE);

    //(test particle)
    double radius = 0.15;
    double xCoord = 0.5;
    double yCoord = 0.8;
    Particle particle(xCoord, yCoord, radius);


    //(main loop)
    while (!glfwWindowShouldClose(window))
    {
        //(reset render buffer for new frame)
        glClear(GL_COLOR_BUFFER_BIT);

        updateCursorCoords(window, cursor);

        drawCircle(particle.get_xCoord(), particle.get_yCoord(), radius, false);
        drawCircle(cursor.get_xCoord(), cursor.get_yCoord(), CURSOR_SIZE, true);

        if (hasCollision(cursor, particle) && !particle.hasCursorCollisionCooldown()) {
            cursorCollision(cursor, particle);
            particle.enableCursorCollisionCooldown();
           //PlaySound(TEXT("SystemStart"), NULL, SND_ALIAS | SND_ASYNC);
        }
        else if (!hasCollision(cursor, particle) && particle.hasCursorCollisionCooldown()) {
            particle.disableCursorCollisionCooldown();
        }

        if (hasBorderCollision(particle)) {
            borderCollision(particle);
            //PlaySound(TEXT("SystemStart"), NULL, SND_ALIAS | SND_ASYNC);
        }
        gravityUpdate(particle);
        movementUpdate(particle);

        //(swap front and back buffers)
        //(Poll for and process events)
        glfwSwapBuffers(window);
        glfwPollEvents();


        printf("y=%.2g, x=%.2g, yVel=%.2g, xVel=%.2g, cursorX=%g, cursorY=%g \n", particle.get_yCoord(), particle.get_xCoord(), particle.get_yVelocity(), particle.get_xVelocity(), cursor_xCoord, cursor_yCoord);
        Sleep(FRAME_TIME);
    }

    glfwTerminate();
    return 0;
};