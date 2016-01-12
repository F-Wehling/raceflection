#include "PhysicsSystem/PhysicsSystem.h"
#include <Main.h>
#include <ObjectSystem/ObjectSystem.h>
#include <btBulletDynamicsCommon.h>
#include <PhysicsSystem/DynamicMotionState.h>
#include <PhysicsSystem/PhysicsSettings.h>

#if OS_WINDOWS
#define M_PI       3.14159265358979323846   // pi
#endif

BEGINNAMESPACE

PhysicsSystem::PhysicsSystem(Main* main):mMain(main), mDynamicsWorld(nullptr), mSolver(nullptr), mDispatcher(nullptr), mCollisionConfiguration(nullptr), mBroadphase(nullptr){
    mSettings = new PhysicsSettings();
    Initialize();
}

PhysicsSystem::~PhysicsSystem(){
    Shutdown();
    delete mSettings;
}

bool PhysicsSystem::Initialize(){
    initBullet();
    
    mRunning = false;
    return true;
}

void PhysicsSystem::initBullet(){
    mBroadphase = new btDbvtBroadphase();
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mSolver = new btSequentialImpulseConstraintSolver;
    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
    mDynamicsWorld->setGravity(mSettings->mGravity);
    mDynamicsWorld->setInternalTickCallback(customTickCallback);
}

void PhysicsSystem::shutdownBullet(){
    if(mDynamicsWorld && mDynamicsWorld != nullptr) delete mDynamicsWorld;
    mDynamicsWorld = nullptr;
    if(mSolver && mSolver != nullptr) delete mSolver;
    mSolver = nullptr;
    if(mDispatcher && mDispatcher != nullptr) delete mDispatcher;
    mDispatcher = nullptr;
    if(mCollisionConfiguration && mCollisionConfiguration != nullptr) delete mCollisionConfiguration;
    mCollisionConfiguration = nullptr;
    if(mBroadphase && mBroadphase != nullptr) delete mBroadphase;
    mBroadphase = nullptr;
}

void PhysicsSystem::Shutdown(){
    clear();
    shutdownBullet();
}

bool PhysicsSystem::tick(float32 dt){
    if(!mRunning)
        return true;
    for(int i = 0; i < mHingeConstraints.size(); ++i){
        HingeConstraint& c = mHingeConstraints[i];
        int indexA = find(c.a);
        int indexB = find(c.b);
        bool activeA = mActive[indexA];
        bool activeB = mActive[indexB];
        if(activeA && activeB){
            if(!c.active){
                c.active = true;
                if(c.constraintObject == nullptr){
                    btVector3 pivotA(c.pivotA[0], c.pivotA[1], c.pivotA[2]);
                    btVector3 pivotB(c.pivotB[0], c.pivotB[1], c.pivotB[2]);
                    btVector3 axisA(c.axisA[0], c.axisA[1], c.axisA[2]);
                    btVector3 axisB(c.axisB[0], c.axisB[1], c.axisB[2]);
                    c.constraintObject = new btHingeConstraint(*mRigidBodies[indexA], *mRigidBodies[indexB], pivotA, pivotB, axisA, axisB);
                    c.constraintObject->setLimit(c.low * M_PI / 180.0, c.high * M_PI / 180.0, mSettings->mHingeSoftness, mSettings->mHingeBias, mSettings->mHingeRelaxation);
                }
                mDynamicsWorld->addConstraint(c.constraintObject, true);
            }
        } else {
            if(c.active){
                c.active = false;
                mDynamicsWorld->removeConstraint(c.constraintObject);
            }
        }
    }
    mDynamicsWorld->stepSimulation(dt / 1000.0, mSettings->mMaximumTicksPerStep, mSettings->mTargetTickLength);
    return true;
}

int PhysicsSystem::find(GameObject* gameObject){
    return find(gameObject->getID());
}

int PhysicsSystem::find(GameObjectId gameObjectId){
    for(int i = 0; i < mGameObjectIds.size(); ++i){
        if(mGameObjectIds[i] == gameObjectId)
            return i;
    }
    return -1;
}

bool PhysicsSystem::contains(GameObject* gameObject){
    return find(gameObject->getID());
}

bool PhysicsSystem::contains(GameObjectId gameObjectId){
    return find(gameObjectId) != -1;
}

void PhysicsSystem::addSphere(GameObject* gameObject, double radius, float mass, float restitution){
    if(contains(gameObject)) return;
    btCollisionShape* collisionShape = new btSphereShape(radius);
    add(gameObject, collisionShape, mass, restitution);
}

void PhysicsSystem::addBox(GameObject* gameObject, Vector3 halfDimensions, float mass, float restitution){
    if(contains(gameObject)) return;
    btCollisionShape* collisionShape = new btBoxShape(btVector3(halfDimensions[0], halfDimensions[1], halfDimensions[2]));
    add(gameObject, collisionShape, mass, restitution);
}

void PhysicsSystem::addCylinderX(GameObject* gameObject, Vector3 halfDimensions, float mass, float restitution){
    if(contains(gameObject)) return;
    btCollisionShape* collisionShape = new btCylinderShapeX(btVector3(halfDimensions[0], halfDimensions[1], halfDimensions[2]));
    add(gameObject, collisionShape, mass, restitution);
}

void PhysicsSystem::addCylinderY(GameObject* gameObject, Vector3 halfDimensions, float mass, float restitution){
    if(contains(gameObject)) return;
    btCollisionShape* collisionShape = new btCylinderShape(btVector3(halfDimensions[0], halfDimensions[1], halfDimensions[2]));
    add(gameObject, collisionShape, mass, restitution);
}

void PhysicsSystem::addCylinderZ(GameObject* gameObject, Vector3 halfDimensions, float mass, float restitution){
    if(contains(gameObject)) return;
    btCollisionShape* collisionShape = new btCylinderShapeZ(btVector3(halfDimensions[0], halfDimensions[1], halfDimensions[2]));
    add(gameObject, collisionShape, mass, restitution);
}

void PhysicsSystem::addPlane(GameObject* gameObject, Vector4 n1n2n3c, float mass, float restitution){
    if(contains(gameObject)) return;
    btCollisionShape* collisionShape = new btStaticPlaneShape(btVector3(n1n2n3c[0], n1n2n3c[1], n1n2n3c[2]), n1n2n3c[3]);
    add(gameObject, collisionShape, mass, restitution);
}

void PhysicsSystem::add(GameObject* gameObject, CollisionTypeFlags type, float* collisionArguments, float mass, float restitution){
    if(contains(gameObject)) return;
    
    //Prepare correct collision shape
    btCollisionShape* collisionShape;
    switch(type){
        case CollisionType::Sphere:
            collisionShape = new btSphereShape(collisionArguments[0]);
            break;
        case CollisionType::Box:
            collisionShape = new btBoxShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case CollisionType::Plane:
            collisionShape = new btStaticPlaneShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]), collisionArguments[3]);
            break;
        case CollisionType::CylinderX:
            collisionShape = new btCylinderShapeX(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case CollisionType::CylinderY:
            collisionShape = new btCylinderShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case CollisionType::CylinderZ:
            collisionShape = new btCylinderShapeZ(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        default:
            return;
    }

    add(gameObject, collisionShape, mass, restitution);
}

void PhysicsSystem::add(GameObject* gameObject, btCollisionShape* collisionShape, float mass, float restitution){
    ObjectSystem* objSystem = mMain->getObjectSystemPtr();
    GameObjectId objId = gameObject->getID();

    //cml::matrix_rotation_quaternion(rotMatrix, parentOrientation);
    Vector3 position = gameObject->getPosition();
    Quaternion orientation = gameObject->getRotation();

    //Prepare motion state
    btTransform initialTransform(btQuaternion(orientation[1], orientation[2], orientation[3], orientation[0]), btVector3(position[0], position[1], position[2]));
    btMotionState* motionState = new DynamicMotionState(objSystem, initialTransform, objId);

    //Prepare rigid body
    btVector3 inertia;
    collisionShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape, inertia);
    constructionInfo.m_restitution = restitution;
    btRigidBody* rigidBody = new btRigidBody(constructionInfo);
    if(objSystem->isTriggerArea(objId))
        rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

    //Add to all lists
    mGameObjectIds.push_back(objId);

    rigidBody->setUserPointer(gameObject); //Used in external callback to get reference to gameObject

    mCollisionShapes.push_back(collisionShape);
    mMotionStates.push_back(motionState);
    mRigidBodies.push_back(rigidBody);
    mActive.push_back(false);
}

void PhysicsSystem::remove(GameObject* gameObject){
    remove(gameObject->getID());
}

void PhysicsSystem::remove(GameObjectId gameObjectId){
    if(!contains(gameObjectId)) return;
    int index = find(gameObjectId);
    for(HingeConstraint& constraint : mHingeConstraints){
        if(constraint.a == gameObjectId || constraint.b == gameObjectId){
            if(constraint.active){
                mDynamicsWorld->removeConstraint(constraint.constraintObject);
                delete constraint.constraintObject;
                constraint.constraintObject = nullptr;
                constraint.active = false;
            }
        }
    }
    mDynamicsWorld->removeRigidBody(mRigidBodies[index]);
    removeIndex(index);
    mGameObjectIds.erase(mGameObjectIds.begin() + index);
}

void PhysicsSystem::removeIndex(int index){
    delete mCollisionShapes[index];
    mCollisionShapes.erase(mCollisionShapes.begin() + index);
    delete mMotionStates[index];
    mMotionStates.erase(mMotionStates.begin() + index);
    delete mRigidBodies[index];
    mRigidBodies.erase(mRigidBodies.begin() + index);
    mActive.erase(mActive.begin() + index);
}

void PhysicsSystem::activate(GameObject* gameObject){
    activate(gameObject->getID());
}

void PhysicsSystem::activate(GameObjectId gameObjectId){
    int index = find(gameObjectId);
    if(index == -1)
        return;
    if(mActive[index])
        return;
    if(mRigidBodies[index]->getInvMass() == 0.0)
        mDynamicsWorld->addRigidBody(mRigidBodies[index], COL_NOMASS, noMassCollidesWith);
    else
        mDynamicsWorld->addRigidBody(mRigidBodies[index], COL_MASS, massCollidesWith);
    mActive[index] = true;
}

void PhysicsSystem::deactivate(GameObject* gameObject){
    deactivate(gameObject->getID());
}

void PhysicsSystem::deactivate(GameObjectId gameObjectId){
    int index = find(gameObjectId);
    if(index == -1)
        return;
    if(!mActive[index])
        return;
    mDynamicsWorld->removeRigidBody(mRigidBodies[index]);
    mActive[index] = false;
}

void PhysicsSystem::start(){
    mRunning = true;
}

void PhysicsSystem::pause(){
    mRunning = false;
}

void PhysicsSystem::clear(){
    removeAllHingeConstraints();
    for(int i = 0; i < mGameObjectIds.size(); ++i){
        mDynamicsWorld->removeRigidBody(mRigidBodies[i]);
        remove(i);
        mGameObjectIds.erase(mGameObjectIds.begin() + i);
        i--;
    }
}

void PhysicsSystem::applyForce(GameObject* gameObject, Vector3 force, Vector3 offsetFromCOMWorldSpace){
    applyForce(gameObject->getID(), force, offsetFromCOMWorldSpace);
}

void PhysicsSystem::applyForce(GameObjectId gameObjectId, Vector3 force, Vector3 offsetFromCOMWorldSpace){
    int index = find(gameObjectId);
    if(index == -1)
        return;
    mRigidBodies[index]->activate();
    mRigidBodies[index]->applyImpulse(btVector3(force[0], force[1], force[2]), btVector3(offsetFromCOMWorldSpace[0], offsetFromCOMWorldSpace[1], offsetFromCOMWorldSpace[2]));
}

void PhysicsSystem::applyTorque(GameObject* gameObject, Vector3 torque){
    applyTorque(gameObject->getID(), torque);
}

void PhysicsSystem::applyTorque(GameObjectId gameObjectId, Vector3 torque){
    int index = find(gameObjectId);
    if(index == -1)
        return;
    mRigidBodies[index]->activate();
    mRigidBodies[index]->applyTorqueImpulse(btVector3(torque[0], torque[1], torque[2]));
}

void PhysicsSystem::applyExplosion(GameObject* bomb, Vector3 bombPos, double maxPower){
    applyExplosion(bomb->getID(), bombPos, maxPower);
}

void PhysicsSystem::applyExplosion(GameObjectId bombId, Vector3 bombPos, double maxPower){
    double maxDist = sqrt(maxPower) - 1;
    //Iterate over all entities, check distance limit, if close enough apply force
    for(int i = 0; i < mGameObjectIds.size(); ++i){
        if(mGameObjectIds[i] == bombId) continue;
        Vector3 objPos = mMain->getObjectSystemPtr()->getObjectByID(mGameObjectIds[i])->getPosition();
        Vector3 dir = objPos - bombPos;
        double dist = dir.length();
        if(dist > maxDist) continue;
        dir /= dist;
        dir *= maxPower / ((1 + dist) * (1 + dist));
        mRigidBodies[i]->activate();
        mRigidBodies[i]->applyImpulse(btVector3(dir[0], dir[1], dir[2]), btVector3(0.0, 0.0, 0.0));
    }
}

void PhysicsSystem::addHingeConstraint(GameObject* a, Vector3 pivotA, Vector3 axisA, GameObject* b, Vector3 pivotB, Vector3 axisB, double low, double high){
    addHingeConstraint(a->getID(), pivotA, axisA, b->getID(), pivotB, axisB, low, high);
}

void PhysicsSystem::addHingeConstraint(GameObjectId a, Vector3 pivotA, Vector3 axisA, GameObjectId b, Vector3 pivotB, Vector3 axisB, double low, double high){
    for(HingeConstraint& constraint : mHingeConstraints){
        if(constraint.a == a && constraint.b == b){
            //LOG("You can't add another hinge constraint between " << a << " and " << b << ". Please remove the old one first.");
            return;
        }
    }
    mHingeConstraints.push_back(HingeConstraint{a, pivotA, axisA, b, pivotB, axisB, low, high, false, nullptr});
}

void PhysicsSystem::removeHingeConstraint(GameObject* a, GameObject* b){
    removeHingeConstraint(a->getID(), b->getID());
}

void PhysicsSystem::removeHingeConstraint(GameObjectId a, GameObjectId b){
    for(std::vector<HingeConstraint>::iterator it = mHingeConstraints.begin(); it != mHingeConstraints.end(); ++it){
        HingeConstraint& constraint = *it;
        if((constraint.a == a && constraint.b == b) || (constraint.a == b && constraint.b == a)){
            if(constraint.active){
                mDynamicsWorld->removeConstraint(constraint.constraintObject);
            }
            if(constraint.constraintObject != nullptr){
                delete constraint.constraintObject;
            }
            it = mHingeConstraints.erase(it);
            break;
        }
    }
}

void PhysicsSystem::removeHingeConstraint(GameObject* a){
    removeHingeConstraint(a->getID());
}

void PhysicsSystem::removeHingeConstraint(GameObjectId a){
    for(std::vector<HingeConstraint>::iterator it = mHingeConstraints.begin(); it != mHingeConstraints.end(); ++it){
        HingeConstraint& constraint = *it;
        if(constraint.a == a || constraint.b == a){
            if(constraint.active){
                mDynamicsWorld->removeConstraint(constraint.constraintObject);
            }
            if(constraint.constraintObject != nullptr){
                delete constraint.constraintObject;
            }
            it = mHingeConstraints.erase(it);
            break;
        }
    }
}

void PhysicsSystem::removeAllHingeConstraints(){
    for(HingeConstraint& constraint : mHingeConstraints){
        if(constraint.active){
            mDynamicsWorld->removeConstraint(constraint.constraintObject);
        }
        if(constraint.constraintObject != nullptr){
            delete constraint.constraintObject;
        }
    }
    mHingeConstraints.clear();
}

void PhysicsSystem::setHingeConstraintLimits(GameObject* a, GameObject* b, double low, double high){
    setHingeConstraintLimits(a->getID(), b->getID(), low, high);
}

void PhysicsSystem::setHingeConstraintLimits(GameObjectId a, GameObjectId b, double low, double high){
    for(HingeConstraint& c : mHingeConstraints){
        if(c.a == a && c.b == b && c.constraintObject != nullptr){
            c.constraintObject->setLimit(low * M_PI / 180.0, high * M_PI / 180.0, mSettings->mHingeSoftness, mSettings->mHingeBias, mSettings->mHingeRelaxation);
        }
    }
}

//TODO: Fix up trigger areas
void customTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    int numManifolds = world->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++)
    {
        btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        if(contactManifold->getNumContacts() == 0) continue; // Filter "almost-collisions"
        GameObject* a = (GameObject*)contactManifold->getBody0()->getUserPointer();
        GameObject* b = (GameObject*)contactManifold->getBody1()->getUserPointer();
        bool isTriggerA = a->getObjectSystem()->isTriggerArea(a->getID());
        bool isTriggerB = b->getObjectSystem()->isTriggerArea(b->getID());
        
        if(isTriggerA){
            if(isTriggerB)
                continue;
            //TODO: Fire Trigger: A triggered B
        } else if(isTriggerB){
            //TODO: Fire Trigger: B triggered A
        }
    }
}

ENDNAMESPACE

