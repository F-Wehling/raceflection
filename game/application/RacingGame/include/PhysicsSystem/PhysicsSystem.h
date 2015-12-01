#pragma once
/*
#include <btBulletDynamicsCommon.h>
#include "DynamicMotionState.h"
#include <unordered_set>
#include <vector>

BEGINNAMESPACE

using EntityId = int;

#define HINGE_SOFTNESS 0.9
#define HINGE_BIAS 0.3
#define HINGE_RELAXATION 1.0

struct HingeConstraint{
    EntityId a;
    Vector3 pivotA, axisA;
    EntityId b;
    Vector3 pivotB, axisB;
    double low, high;
    bool inWorld;
    btHingeConstraint* constraintObject;
};

struct MassChange{
    int index;
    double mass;
};

//Represents a pair of entities that should not collide
struct PreventCollisionPair{
    EntityId a, b;
    
    inline PreventCollisionPair(EntityId _a, EntityId _b): a(_a), b(_b) {}
    
};

//Equals operator for PreventCollisionPairs for usage in unordered_set
inline bool operator==(const PreventCollisionPair& lhs, const PreventCollisionPair& rhs){
    return lhs.a == rhs.a && lhs.b == rhs.b;
}

//Hash function for PreventCollisionPairs for usage in unordered_set
namespace std {
    template<>
    struct hash<PreventCollisionPair>{
        typedef PreventCollisionPair argument_type;
        typedef std::size_t result_type;
        
        result_type operator()(argument_type const& s) const {
            result_type const h1 ( std::hash<int>()(s.a));
            result_type const h2 ( std::hash<int>()(s.b));
            return h1 ^ (h2 << 1);
        }
    };
}

//Represents the Physics subsystem. Organizes all interaction with and all callbacks from the Bullet Physics Engine.
class PhysicsSystem {
    
public:
    //Flag for an entity: NONE - should be removed from physics, KEEP - keep in memory, but don't simulate, SIMULATE - simulate physics
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
    PhysicsSystem();
public:
    //Initializes the subsystem
    bool Initialize();
    //Initializes the Bullet btDynamicsWorld and all settings
    void initBullet();
    //Shuts down and deletes the btDynamicsWorld
    void shutdownBullet();
    //Shuts down the subsystem
    void Shutdown();
    //Update function from the engine, called every frame. Will process all entities and call the required simulation in Bullet
    bool Update(float dt, float fullDt);
    
private:
    //Bullet world objects, required for bullet to work. Pointer must be deleted manually (in shutdownBullet()).
    btBroadphaseInterface* mBroadphase;
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;
    btSequentialImpulseConstraintSolver* mSolver;
    btDiscreteDynamicsWorld* mDynamicsWorld;
    
    //List of all entities, indices will be used for all the other lists too
    std::vector<EntityId> mEntities;
    //List of all collision shapes. Pointers must be deleted manually (in remove(...)).
    std::vector<btCollisionShape*> mCollisionShapes;
    //List of all motion states. Pointers must be deleted manually (in remove(...)).
    std::vector<btMotionState*> mMotionStates;
    //List of all rigid bodies. Pointers must be deleted manually (in remove(...)).
    std::vector<btRigidBody*> mRigidBodies;
    //List of all entity flags determining whether to simulate an entity or not.
    std::vector<Flag> mFlags;
    //If the entity is within the btDynamicsWorld right now
    std::vector<bool> mInWorld;
    //If the entity is a ghost object
    std::vector<bool> mIsGhost;
    
    //Set of all ConveyorBelt ids, for their collision detection
    std::unordered_set<EntityId> mConveyorBelts;
    //Set of all collision exceptions, used for temporarily adding exception to collision
    std::unordered_set<PreventCollisionPair> mCollisionExceptions;
    //List of Hinge constraints
    std::vector<HingeConstraint> mHingeConstraints;
    //Todo mass changes
    std::vector<MassChange> mMassChanges;
    
    //Default values for objects without parents
    Vector3 defaultPosition;
    Quaternion defaultOrientation;
    
    //Whether the physics is running right now
    bool mRunning;
    
    //Utility, return index of entity in all lists.
    int find(EntityId entity);
    //Removes the entity at the index and deletes all pointers.
    void remove(int index);
    
public:
    //Builds a waterproof room to enclose the level. This contains physic only entities for all permanently static objects like walls and table.
    void buildWaterproofRoom();
    //Returns whether the subsystem already contains data for this entity
    bool contains(artemis::Entity& entity);
    //Sets the flag for an entity
    void flag(artemis::Entity& entity, Flag flag);
    //Adds an entity to the subsystem
    void add(artemis::Entity& entity, component::CollisionType type, std::vector<float> collisionArguments, float mass, float restitution, bool ghost, bool movable, Vector3D &position, Vector3D &resetPosition, Quaternion &orientation, Quaternion &resetOrientation);
    //Adds an entity which is part of a compound objects to the subsystem
    void add(artemis::Entity& entity, component::CollisionType type, std::vector<float> collisionArguments, float mass, float restitution, bool ghost, bool movable, Vector3D& parentPosition, Vector3D &position, Vector3D &resetPosition, Quaternion &parentOrientation, Quaternion &orientation, Quaternion &resetOrientation);
    //Removes an entity from the subsystem
    void remove(artemis::Entity& entity);
    
    //Starts the physics
    void start();
    //Pauses the physics
    void pause();
    //Clears everything, removes all entities and resets the world
    void clear();
    //Resets the world to its initial state.
    void reset();
    
    //Applies a force to an entity
    void applyForce(EntityId entityID, Vector3 force, Vector3 offsetFromCOMWorldSpace = Vector3(0.0, 0.0, 0.0));
    //Applies a torque to an entity
    void applyTorque(EntityId entityID, Vector3 torque);
    //Change the mass of an object during runtime
    void setMass(EntityId entityId, double mass);
    //Applies an explosion from an entity with a certain power
    void applyExplosion(EntityId bomb, Vector3 bombPos, double maxPower);
    //Marks an entity as conveyor belt with a certain power
    void addConveyorBelt(EntityId belt, Vector3 force);
    //Removes the conveyor belt property from an entity
    void removeConveyorBelt(EntityId belt);
    //Checks if an entity is marked as conveyor belt
    bool isConveyorBelt(EntityId belt);
    //Gets the power of a conveyor belt entity
    btVector3 getLinearVelocity(EntityId belt);
    
    //Checks if an entity is a ghost object
    bool isGhostObject(EntityId ghost);
    
    //Adds a collision exception to the physics. Entity pairs added will not collide.
    void addCollisionException(EntityId a, EntityId b);
    //Removes a collision exception. Entity pairs will now collide again.
    void removeCollisionException(EntityId a, EntityId b);
    
    //Adds a hinge constraint to the physics. Will only activate if oth entities are in world, low / high in degrees
    void addHingeConstraint(EntityId a, Vector3 pivotA, Vector3 axisA, EntityId b, Vector3 pivotB, Vector3 axisB, double low, double high);
    //Removes the hinge constraint corresponding to a and b from the physics.
    void removeHingeConstraint(EntityId a, EntityId b);
    //Removes all hinge constraint corresponding to a  from the physics.
    void removeHingeConstraint(EntityId a);
    //Removes all registered hinge constraints
    void removeAllHingeConstraints();
    //Change a HingeConstraints limits, low / high in degrees
    void setHingeConstraintLimits(EntityId a, EntityId b, double low, double high);
    
    //Return the vector of all rigid bodies (used for external collision callback)
    const std::vector<btRigidBody*>& getRigidBodies(){return mRigidBodies;}
    //Returns vector of all entities (used for external collision callback)
    const std::vector<EntityId>& getEntities(){return mEntities;}
    //Returns the set of all collision exceptions (used for external collision callback)
    std::unordered_set<PreventCollisionPair>& getCollisionExceptions(){return mCollisionExceptions;}
};

//Custom callback in the near phase of collisions. Filters all collisions that are in the exception set.
void customNearCallback(btBroadphasePair& collisionPair, btCollisionDispatcher& dispatcher, const btDispatcherInfo& dispatcherInfo);
//Custom callback in the collision phase. Applies necessary forces for conveyor belts
bool CustomMaterialCombinerCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1);
//Custom tick callback for checking ghost objects
void customTickCallback(btDynamicsWorld *world, btScalar timeStep);

ENDNAMESPACE
*/
