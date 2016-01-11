#pragma once

#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

class btDynamicsWorld;
class btRigidBody;
class btVehicleRaycaster;

BEGINNAMESPACE

class Vehicle {
private:
    btVehicleRaycaster *mRaycaster;
    btRaycastVehicle::btVehicleTuning mTuning;
    btRaycastVehicle *mVehicle;

    double mSteering;
    double mSteeringClamp;
    double mSteeringIncrement; // degree/sec
public:
    Vehicle(btDynamicsWorld *world, btRigidBody *chassis);
    ~Vehicle();

    //For each button: 0.0d = not pressed, 1.0d fully pressed, supports analog controls
    void updateControls (double dt, double forward, double reverse, double left, double right);

    double getSpeed();
};

ENDNAMESPACE
