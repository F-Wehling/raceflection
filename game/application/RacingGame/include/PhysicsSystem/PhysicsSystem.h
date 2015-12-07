#pragma once

#include <btBulletDynamicsCommon.h>
#include <PhysicsSystem/DynamicMotionState.h>
#include <unordered_set>
#include <vector>
#include <Main.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <ObjectSystem/ObjectSystem.h>

BEGINNAMESPACE

using GameObjectId = int;
using Vector3 = glm::vec3;
using Matrix4x4 = glm::mat4x4;
using Quaternion = glm::quat;

#define HINGE_SOFTNESS 0.9
#define HINGE_BIAS 0.3
#define HINGE_RELAXATION 1.0

struct HingeConstraint{
    GameObjectId a;
    Vector3 pivotA, axisA;
    GameObjectId b;
    Vector3 pivotB, axisB;
    double low, high;
    bool inWorld;
    btHingeConstraint* constraintObject;
};

struct MassChange{
    int index;
    double mass;
};

enum CollisionType{
    SPHERE, BOX, PLANE, CYLINDERX, CYLINDERY, CYLINDERZ
};

//Represents the Physics subsystem. Organizes all interaction with and all callbacks from the Bullet Physics Engine.
class PhysicsSystem {
    
public:
    //Flag for an gameObject: NONE - should be removed from physics, KEEP - keep in memory, but don't simulate, SIMULATE - simulate physics
    enum Flag{
        NONE, KEEP, SIMULATE
    };
    
    //Different collision classes, used to not have collision calculations between f.e. static objects
#define BIT(x) (1<<(x))
    enum collisiontypes {
        COL_NOTHING = 0, //Collide with nothing
        COL_NOMASS = BIT(0), //Collide with static objects
        COL_MASS = BIT(1), //Collide with dynamic objects
    };
    
    int massCollidesWith = COL_NOMASS | COL_MASS; //Dynamic objects collide with everything
    int noMassCollidesWith = COL_MASS; //Static objects only collide with dynamic objects
    
private:
    Main* mMain;

    PhysicsSystem(Main* main);
public:
    //Initializes the subsystem
    bool Initialize();
    //Initializes the Bullet btDynamicsWorld and all settings
    void initBullet();
    //Shuts down and deletes the btDynamicsWorld
    void shutdownBullet();
    //Shuts down the subsystem
    void Shutdown();
    //Update function from the engine, called every frame. Will process all gameObjects and call the required simulation in Bullet
    bool Update(float dt, float fullDt);
    
private:
    //Bullet world objects, required for bullet to work. Pointer must be deleted manually (in shutdownBullet()).
    btBroadphaseInterface* mBroadphase;
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;
    btSequentialImpulseConstraintSolver* mSolver;
    btDiscreteDynamicsWorld* mDynamicsWorld;
    
    //List of all gameObjects, indices will be used for all the other lists too
    std::vector<GameObjectId> mGameObjects;
    //List of all collision shapes. Pointers must be deleted manually (in remove(...)).
    std::vector<btCollisionShape*> mCollisionShapes;
    //List of all motion states. Pointers must be deleted manually (in remove(...)).
    std::vector<btMotionState*> mMotionStates;
    //List of all rigid bodies. Pointers must be deleted manually (in remove(...)).
    std::vector<btRigidBody*> mRigidBodies;
    //If the gameObject is within the btDynamicsWorld right now
    std::vector<bool> mInWorld;

    //List of Hinge constraints
    std::vector<HingeConstraint> mHingeConstraints;
    
    //Whether the physics is running right now
    bool mRunning;
    
    //Utility, return index of gameObject in all lists.
    int find(GameObjectId gameObject);
    //Removes the gameObject at the index and deletes all pointers.
    void remove(int index);
    
public:
    //Returns whether the subsystem already contains data for this gameObject
    bool contains(GameObject& gameObject);
    //Adds an gameObject to the subsystem
    void add(GameObject& gameObject, CollisionType type, std::vector<float> collisionArguments, float mass, float restitution);
    //Removes an gameObject from the subsystem
    void remove(GameObject& gameObject);
    
    //Starts the physics
    void start();
    //Pauses the physics
    void pause();
    //Clears everything, removes all gameObjects and resets the world
    void clear();
    //Resets the world to its initial state.
    void reset();
    
    //Applies a force to an gameObject
    void applyForce(GameObjectId gameObjectId, Vector3 force, Vector3 offsetFromCOMWorldSpace = Vector3(0.0, 0.0, 0.0));
    //Applies a torque to an gameObject
    void applyTorque(GameObjectId gameObjectId, Vector3 torque);
    //Applies an explosion from an gameObject with a certain power
    void applyExplosion(GameObjectId bomb, Vector3 bombPos, double maxPower);
    //Marks an gameObject as conveyor belt with a certain power

    //TODO: Add functions to change collision flags of objects during runtime
    
    //Adds a hinge constraint to the physics. Will only activate if both gameObjects are in world, low / high in degrees
    void addHingeConstraint(GameObjectId a, Vector3 pivotA, Vector3 axisA, GameObjectId b, Vector3 pivotB, Vector3 axisB, double low, double high);
    //Removes the hinge constraint corresponding to a and b from the physics.
    void removeHingeConstraint(GameObjectId a, GameObjectId b);
    //Removes all hinge constraint corresponding to a  from the physics.
    void removeHingeConstraint(GameObjectId a);
    //Removes all registered hinge constraints
    void removeAllHingeConstraints();
    //Change a HingeConstraints limits, low / high in degrees
    void setHingeConstraintLimits(GameObjectId a, GameObjectId b, double low, double high);
};

//Custom tick callback for checking ghost objects
void customTickCallback(btDynamicsWorld *world, btScalar timeStep);

ENDNAMESPACE
