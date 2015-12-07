#include <PhysicsSystem/Vehicle.h>

BEGINNAMESPACE

Vehicle::Vehicle(btDynamicsWorld* world, btRigidBody* chassis){
    m_Raycaster = new btDefaultVehicleRaycaster(world);
    m_Vehicle = new btRaycastVehicle(m_Tuning, chassis, m_Raycaster);

    m_Steering = 0.0;
    m_SteeringClamp = 45.0;
    m_SteeringIncrement = 120.0;
}

Vehicle::~Vehicle(){
    delete m_Vehicle;
    delete m_Raycaster;
}

void Vehicle::updateControls(double dt, double forward, double reverse, double left, double right){
    double engineForce = 1000.0 * forward;
    double brakeForce = 100.0 * reverse;

    m_Steering += left * dt * m_SteeringIncrement;
    m_Steering = fmin(m_Steering, m_SteeringClamp);

    m_Steering -= right * dt * m_SteeringIncrement;
    m_Steering = fmax(m_Steering, -m_SteeringClamp);

    m_Vehicle->setSteeringValue(m_Steering, 0);
    m_Vehicle->setSteeringValue(m_Steering, 1);

    m_Vehicle->applyEngineForce(engineForce, 2);
    m_Vehicle->applyEngineForce(engineForce, 3);
    m_Vehicle->setBrake(brakeForce, 2);
    m_Vehicle->setBrake(brakeForce, 3);
}

ENDNAMESPACE
