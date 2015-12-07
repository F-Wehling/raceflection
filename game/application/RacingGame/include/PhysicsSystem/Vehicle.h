#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

BEGINNAMESPACE

class Vehicle /*: public Entity*/ {
private:
    btVehicleRaycaster* m_Raycaster;
    btRaycastVehicle::btVehicleTuning m_Tuning;
    btRaycastVehicle* m_Vehicle;

    double m_Steering;
    double m_SteeringClamp;
    double m_SteeringIncrement; // degree/sec
public:
    Vehicle(btDynamicsWorld* world, btRigidBody* chassis);
    ~Vehicle();

    //For each button: 0.0d = not pressed, 1.0d fully pressed, supports analog controls
    void updateControls (double dt, double forward, double reverse, double left, double right);


};

ENDNAMESPACE
