#include "PhysicsSystem/PhysicsSystem.h"

BEGINNAMESPACE
PhysicsSystem::PhysicsSystem(Main* main):mMain(main), mDynamicsWorld(nullptr), mSolver(nullptr), mDispatcher(nullptr), mCollisionConfiguration(nullptr), mBroadphase(nullptr){}

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
    mDynamicsWorld->setGravity(btVector3(0, -9.8, 0));
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

bool PhysicsSystem::Update(float32 dt, float32 fullDt){
    if(!mRunning)
        return true;
    //Check all entities
    //TODO: Extract into functions for general use
    /*for(int i = 0; i < mGameObjects.size(); ++i){
        switch(mFlags[i]){
            case Flag::NONE: //Should not be kept, remove from subsystem
                remove(i);
                mGameObjects.erase(mGameObjects.begin() + i);
                break;
            case Flag::KEEP: //Should only be kept, if in physics world, remove it from there
                if(mInWorld[i]){
                    mDynamicsWorld->removeRigidBody(mRigidBodies[i]);
                    mInWorld[i] = false;
                }
                break;
            case Flag::SIMULATE: //Should be simulated, if not in physics world, add it
                if(!mInWorld[i]){
                    if(mRigidBodies[i]->getInvMass() == 0.0)
                        mDynamicsWorld->addRigidBody(mRigidBodies[i], COL_NOMASS, noMassCollidesWith);
                    else
                        mDynamicsWorld->addRigidBody(mRigidBodies[i], COL_MASS, massCollidesWith);
                    mInWorld[i] = true;
                }
                break;
        }
        mFlags[i] = Flag::NONE; //Reset flag afterwards, this has to be set by Entity subsystem each update
    }
    for(int i = 0; i < mHingeConstraints.size(); ++i){
        HingeConstraint& c = mHingeConstraints[i];
        int indexA = find(c.a);
        int indexB = find(c.b);
        bool inWorldA = mInWorld[indexA];
        bool inWorldB = mInWorld[indexB];
        if(inWorldA && inWorldB){
            if(!c.inWorld){
                c.inWorld = true;
                if(c.constraintObject == nullptr){
                    btVector3 pivotA(c.pivotA[0], c.pivotA[1], c.pivotA[2]);
                    btVector3 pivotB(c.pivotB[0], c.pivotB[1], c.pivotB[2]);
                    btVector3 axisA(c.axisA[0], c.axisA[1], c.axisA[2]);
                    btVector3 axisB(c.axisB[0], c.axisB[1], c.axisB[2]);
                    c.constraintObject = new btHingeConstraint(*mRigidBodies[indexA], *mRigidBodies[indexB], pivotA, pivotB, axisA, axisB);
                    c.constraintObject->setLimit(c.low * M_PI / 180.0, c.high * M_PI / 180.0, HINGE_SOFTNESS, HINGE_BIAS, HINGE_RELAXATION);
                }
                mDynamicsWorld->addConstraint(c.constraintObject, true);
            }
        } else {
            if(c.inWorld){
                c.inWorld = false;
                mDynamicsWorld->removeConstraint(c.constraintObject);
            }
        }
    }*/

    mDynamicsWorld->stepSimulation(dt / 1000.0, 20, 1.0 / 120.0);
    return true;
}

int PhysicsSystem::find(GameObjectId gameObject){
    for(int i = 0; i < mGameObjects.size(); ++i){
        if(mGameObjects[i] == gameObject)
            return i;
    }
    return -1;
}

bool PhysicsSystem::contains(GameObject& gameObject){
    return find(gameObject.getID()) != -1;
}

void PhysicsSystem::add(GameObject& gameObject, CollisionType type, std::vector<float> collisionArguments, float mass, float restitution){
    if(contains(gameObject)) return;

    ObjectSystem* objSystem = mMain->getObjectSystemPtr();
    GameObjectId objId = gameObject.getID();
    
    //Prepare correct collision shape
    btCollisionShape* collisionShape;
    switch(type){
        case CollisionType::SPHERE:
            collisionShape = new btSphereShape(collisionArguments[0]);
            break;
        case CollisionType::BOX:
            collisionShape = new btBoxShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case CollisionType::PLANE:
            collisionShape = new btStaticPlaneShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]), collisionArguments[3]);
            break;
        case CollisionType::CYLINDERX:
            collisionShape = new btCylinderShapeX(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case CollisionType::CYLINDERY:
            collisionShape = new btCylinderShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case CollisionType::CYLINDERZ:
            collisionShape = new btCylinderShapeZ(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        default:
            return;
    }

    //cml::matrix_rotation_quaternion(rotMatrix, parentOrientation);
    Vector3 position = gameObject.getPosition();
    Quaternion orientation = gameObject.getRotation();
    
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
    mGameObjects.push_back(objId);
    
    rigidBody->setUserPointer(&gameObject); //Used in external callback to get reference to gameObject
    
    mCollisionShapes.push_back(collisionShape);
    mMotionStates.push_back(motionState);
    mRigidBodies.push_back(rigidBody);
    mInWorld.push_back(false);
}

void PhysicsSystem::remove(GameObject& gameObject){
    if(!contains(gameObject)) return;
    GameObjectId objId = gameObject.getID();
    int index = find(objId);
    for(HingeConstraint& constraint : mHingeConstraints){
        if(constraint.a == objId || constraint.b == objId){
            if(constraint.inWorld){
                mDynamicsWorld->removeConstraint(constraint.constraintObject);
                delete constraint.constraintObject;
                constraint.constraintObject = nullptr;
                constraint.inWorld = false;
            }
        }
    }
    mDynamicsWorld->removeRigidBody(mRigidBodies[index]);
    remove(index);
    mGameObjects.erase(mGameObjects.begin() + index);
}

void PhysicsSystem::remove(int index){
    delete mCollisionShapes[index];
    mCollisionShapes.erase(mCollisionShapes.begin() + index);
    delete mMotionStates[index];
    mMotionStates.erase(mMotionStates.begin() + index);
    delete mRigidBodies[index];
    mRigidBodies.erase(mRigidBodies.begin() + index);
    mInWorld.erase(mInWorld.begin() + index);
}

void PhysicsSystem::start(){
    mRunning = true;
}

void PhysicsSystem::pause(){
    mRunning = false;
}

void PhysicsSystem::clear(){
    removeAllHingeConstraints();
    for(int i = 0; i < mGameObjects.size(); ++i){
        mDynamicsWorld->removeRigidBody(mRigidBodies[i]);
        remove(i);
        mGameObjects.erase(mGameObjects.begin() + i);
        i--;
    }
}

/*void PhysicsSystem::reset(){
    mRunning = false;
    for(HingeConstraint& constraint : mHingeConstraints){
        if(constraint.inWorld){
            constraint.inWorld = false;
            mDynamicsWorld->removeConstraint(constraint.constraintObject);
        }
        if(constraint.constraintObject != nullptr){
            delete constraint.constraintObject;
            constraint.constraintObject = nullptr;
        }
    }
    for(int i = 0; i < mGameObjects.size(); ++i){
        static_cast<DynamicMotionState*>(mMotionStates[i])->reset();
        remove(i);
        mGameObjects.erase(mGameObjects.begin() + i);
        i--;
    }
    shutdownBullet();
    initBullet();
}*/

void PhysicsSystem::applyForce(GameObjectId gameObjectId, Vector3 force, Vector3 offsetFromCOMWorldSpace){
    int index = find(gameObjectId);
    if(index == -1)
        return;
    mRigidBodies[index]->activate();
    mRigidBodies[index]->applyImpulse(btVector3(force[0], force[1], force[2]), btVector3(offsetFromCOMWorldSpace[0], offsetFromCOMWorldSpace[1], offsetFromCOMWorldSpace[2]));
}

void PhysicsSystem::applyTorque(GameObjectId gameObjectId, Vector3 torque){
    int index = find(gameObjectId);
    if(index == -1)
        return;
    mRigidBodies[index]->activate();
    mRigidBodies[index]->applyTorqueImpulse(btVector3(torque[0], torque[1], torque[2]));
}

void PhysicsSystem::applyExplosion(GameObjectId bomb, Vector3 bombPos, double maxPower){
    double maxDist = sqrt(maxPower) - 1;
    //Iterate over all entities, check distance limit, if close enough apply force
    for(int i = 0; i < mGameObjects.size(); ++i){
        if(mGameObjects[i] == bomb) continue;
        Vector3 objPos = mMain->getObjectSystemPtr()->getObjectByID(mGameObjects[i])->getPosition();
        Vector3 dir = objPos - bombPos;
        double dist = dir.length();
        if(dist > maxDist) continue;
        dir /= dist;
        dir *= maxPower / ((1 + dist) * (1 + dist));
        mRigidBodies[i]->activate();
        mRigidBodies[i]->applyImpulse(btVector3(dir[0], dir[1], dir[2]), btVector3(0.0, 0.0, 0.0));
    }
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

void PhysicsSystem::removeHingeConstraint(GameObjectId a, GameObjectId b){
    for(std::vector<HingeConstraint>::iterator it = mHingeConstraints.begin(); it != mHingeConstraints.end(); ++it){
        HingeConstraint& constraint = *it;
        if((constraint.a == a && constraint.b == b) || (constraint.a == b && constraint.b == a)){
            if(constraint.inWorld){
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

void PhysicsSystem::removeHingeConstraint(GameObjectId a){
    for(std::vector<HingeConstraint>::iterator it = mHingeConstraints.begin(); it != mHingeConstraints.end(); ++it){
        HingeConstraint& constraint = *it;
        if(constraint.a == a || constraint.b == a){
            if(constraint.inWorld){
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
        if(constraint.inWorld){
            mDynamicsWorld->removeConstraint(constraint.constraintObject);
        }
        if(constraint.constraintObject != nullptr){
            delete constraint.constraintObject;
        }
    }
    mHingeConstraints.clear();
}

void PhysicsSystem::setHingeConstraintLimits(GameObjectId a, GameObjectId b, double low, double high){
    for(HingeConstraint& c : mHingeConstraints){
        if(c.a == a && c.b == b && c.constraintObject != nullptr){
            c.constraintObject->setLimit(low * M_PI / 180.0, high * M_PI / 180.0, HINGE_SOFTNESS, HINGE_BIAS, HINGE_RELAXATION);
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

