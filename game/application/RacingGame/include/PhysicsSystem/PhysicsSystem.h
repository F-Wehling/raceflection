#pragma once

#include <unordered_set>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <LinearMath/btScalar.h>
#include <PhysicsSpec.h>

class btHingeConstraint;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btRigidBody;
class btMotionState;
class btCollisionShape;

BEGINNAMESPACE

class Main;
class GameObject;
class PhysicsSettings;

using GameObjectId = int;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix4x4 = glm::mat4x4;
using Quaternion = glm::quat;

struct HingeConstraint{
    GameObjectId a;
    Vector3 pivotA, axisA;
    GameObjectId b;
    Vector3 pivotB, axisB;
    double low, high;
    bool active;
    btHingeConstraint* constraintObject;
};

//Represents the Physics subsystem. Organizes all interaction with and all callbacks from the Bullet Physics Engine.
class PhysicsSystem {
    
public:
    
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

    //Initializes the system
    bool Initialize();

public:
    PhysicsSystem(Main* main);
    ~PhysicsSystem();

    //Shuts down the system, called in destructor
    void Shutdown();
    //Update function from the engine, called every frame. Will process all hinge constraints and call the required simulation in Bullet
    bool tick(float dt);
    
private:
    //Settings for physics
    PhysicsSettings* mSettings;

    //Bullet world objects, required for bullet to work. Pointer must be deleted manually (in shutdownBullet()).
    btBroadphaseInterface* mBroadphase;
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;
    btSequentialImpulseConstraintSolver* mSolver;
    btDiscreteDynamicsWorld* mDynamicsWorld;
    
    //List of all gameObjects, indices will be used for all the other lists too
    std::vector<GameObjectId> mGameObjectIds;
    //List of all collision shapes. Pointers must be deleted manually (in remove(...)).
    std::vector<btCollisionShape*> mCollisionShapes;
    //List of all motion states. Pointers must be deleted manually (in remove(...)).
    std::vector<btMotionState*> mMotionStates;
    //List of all rigid bodies. Pointers must be deleted manually (in remove(...)).
    std::vector<btRigidBody*> mRigidBodies;
    //If the gameObject is within the btDynamicsWorld right now
    std::vector<bool> mActive;

    //List of Hinge constraints
    std::vector<HingeConstraint> mHingeConstraints;
    
    //Whether the physics is running right now
    bool mRunning;

    //Initializes the Bullet btDynamicsWorld and all settings
    void initBullet();
    //Shuts down and deletes the btDynamicsWorld
    void shutdownBullet();
    
    //Utility, return index of game object in all lists.
    int find(GameObjectId gameObjectId);
    //Removes the game object at the index and deletes all pointers.
    void remove(int index);
    //Adds a game object to the system, has to be activated afterwards
    void add(GameObject& gameObject, btCollisionShape* collisionShape, float mass, float restitution);
    
public:
    //Adds a spherical game object to the system, has to be activated afterwards
    void addSphere(GameObject& gameObject, double radius, float mass, float restitution);
    //Adds a box-like game object to the system, has to be activated afterwards
    void addBox(GameObject& gameObject, Vector3 halfDimensions, float mass, float restitution);
    //Adds a cylindrical game object to the system, circular around its x axis, has to be activated afterwards
    void addCylinderX(GameObject& gameObject, Vector3 halfDimensions, float mass, float restitution);
    //Adds a cylindrical game object to the system, circular around its y axis, has to be activated afterwards
    void addCylinderY(GameObject& gameObject, Vector3 halfDimensions, float mass, float restitution);
    //Adds a cylindrical game object to the system, circular around its z axis, has to be activated afterwards
    void addCylinderZ(GameObject& gameObject, Vector3 halfDimensions, float mass, float restitution);
    //Adds a planar game object to the system, where the plane is defined by (n1 n2 n3) * x = c, has to be activated afterwards
    void addPlane(GameObject& gameObject, Vector4 n1n2n3c, float mass, float restitution);
    //Adds a game object to the system, has to be activated afterwards
    void add(GameObject& gameObject, CollisionTypeFlags type, float* collisionArguments, float mass, float restitution);
    //Removes an game object from the system
    void remove(GameObject& gameObject);
    //Returns whether the system already contains data for this game object
    bool contains(GameObject& gameObject);

    //Activates physics for a game object
    void activate(GameObjectId gameObjectId);
    //Deactivates physics for a game object
    void deactivate(GameObjectId gameObjectId);
    
    //Starts the physics
    void start();
    //Pauses the physics
    void pause();
    //Clears everything, removes all game objects
    void clear();
    
    //Applies a force to an game object
    void applyForce(GameObjectId gameObjectId, Vector3 force, Vector3 offsetFromCOMWorldSpace = Vector3(0.0, 0.0, 0.0));
    //Applies a torque to an game object
    void applyTorque(GameObjectId gameObjectId, Vector3 torque);
    //Applies an explosion from an game object with a certain power
    void applyExplosion(GameObjectId bombId, Vector3 bombPos, double maxPower);

    //TODO: Add functions to change collision flags of objects during runtime
    
    //Adds a hinge constraint to the physics. Will only activate if both game objects are in world, low / high in degrees
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

//Custom tick callback for checking trigger area objects
void customTickCallback(btDynamicsWorld *world, btScalar timeStep);

ENDNAMESPACE
