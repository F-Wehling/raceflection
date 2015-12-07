#include <PhysicsSystem/Vehicle.h>

#include <BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

BEGINNAMESPACE

Vehicle::Vehicle(btDynamicsWorld* world, btRigidBody* chassis){
    mRaycaster = new btDefaultVehicleRaycaster(world);
    mVehicle = new btRaycastVehicle(mTuning, chassis, mRaycaster);

    mSteering = 0.0;
    mSteeringClamp = 45.0;
    mSteeringIncrement = 120.0;
}

Vehicle::~Vehicle(){
    delete mVehicle;
    delete mRaycaster;
}

void Vehicle::updateControls(double dt, double forward, double reverse, double left, double right){
    double engineForce = 1000.0 * forward;
    double brakeForce = 100.0 * reverse;

    mSteering += left * dt * mSteeringIncrement;
    mSteering = fmin(mSteering, mSteeringClamp);

    mSteering -= right * dt * mSteeringIncrement;
    mSteering = fmax(mSteering, -mSteeringClamp);

    mVehicle->setSteeringValue(mSteering, 0);
    mVehicle->setSteeringValue(mSteering, 1);

    mVehicle->applyEngineForce(engineForce, 2);
    mVehicle->applyEngineForce(engineForce, 3);
    mVehicle->setBrake(brakeForce, 2);
    mVehicle->setBrake(brakeForce, 3);
}

ENDNAMESPACE
