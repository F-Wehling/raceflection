#include "PhysicsSystem/PhysicsSystem.h"
/*

BEGINNAMESPACE
PhysicsSystem::PhysicsSystem(Main* main):mMain(main), mDynamicsWorld(nullptr), mSolver(nullptr), mDispatcher(nullptr), mCollisionConfiguration(nullptr), mBroadphase(nullptr){}

bool PhysicsSystem::Initialize(){
    defaultPosition = cml::zero<3>();
    defaultOrientation.identity();
    
    initBullet();
    
    buildWaterproofRoom();
    
    gContactAddedCallback = CustomMaterialCombinerCallback;
    
    mRunning = false;
    return true;
}

void PhysicsSystem::initBullet(){
    mBroadphase = new btDbvtBroadphase();
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mDispatcher->setNearCallback(customNearCallback);
    mSolver = new btSequentialImpulseConstraintSolver;
    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
    mDynamicsWorld->setGravity(btVector3(0, -9.8, 0));
    mDynamicsWorld->getSolverInfo().m_solverMode |= SOLVER_ENABLE_FRICTION_DIRECTION_CACHING;
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
    for(int i = 0; i < mMassChanges.size(); ++i){
        int index = mMassChanges[i].index;
        double mass = mMassChanges[i].mass;
        btVector3 inertia;
        if(mInWorld[index])
            mDynamicsWorld->removeRigidBody(mRigidBodies[index]);
        mRigidBodies[index]->getCollisionShape()->calculateLocalInertia(mass, inertia);
        mRigidBodies[index]->setMassProps(mass, inertia);
        mRigidBodies[index]->updateInertiaTensor();
        if(mInWorld[index]){
            if(mRigidBodies[index]->getInvMass() == 0.0)
                mDynamicsWorld->addRigidBody(mRigidBodies[index], COL_NOMASS, noMassCollidesWith);
            else
                mDynamicsWorld->addRigidBody(mRigidBodies[index], COL_MASS, massCollidesWith);
            mRigidBodies[index]->activate();
        }

    }
    mMassChanges.clear();
    //Check all entities
    for(int i = 0; i < mEntities.size(); ++i){
        switch(mFlags[i]){
            case Flag::NONE: //Should not be kept, remove from subsystem
                remove(i);
                mEntities.erase(mEntities.begin() + i);
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
    }
    
#if SIMULATION
    mDynamicsWorld->stepSimulation(1/60.0,6,1/60.0);
#else
    //Simulate physics at 2/3 of real speed (better looks) at 30 internal ticks per sec
    mDynamicsWorld->stepSimulation(1.0 / 3.0 * dt / 1000.0, 10, 1.0 / 60.0);
#endif
    return true;
}

int PhysicsSystem::find(EntityId entity){
    for(int i = 0; i < mEntities.size(); ++i){
        if(mEntities[i] == entity)
            return i;
    }
    return -1;
}

bool PhysicsSystem::contains(artemis::Entity& entity){
    return find(entity.getUniqueId()) != -1;
}

void PhysicsSystem::flag(artemis::Entity& entity, Flag flag){
    if(!contains(entity)) return;
    mFlags[find(entity.getUniqueId())] = flag;
}

void PhysicsSystem::add(artemis::Entity& entity, component::CollisionType type, std::vector<float> collisionArguments, float mass, float restitution, bool ghost, bool movable, Vector3 &position, Vector3 &resetPosition, Quaternion &orientation, Quaternion &resetOrientation){
    //To add an entity without parent just add it with default parent
    add(entity, type, collisionArguments, mass, restitution, ghost, movable, defaultPosition, position, resetPosition, defaultOrientation, orientation, resetOrientation);
}

void PhysicsSystem::add(artemis::Entity& entity, component::CollisionType type, std::vector<float> collisionArguments, float mass, float restitution, bool ghost, bool movable, Vector3& parentPosition, Vector3 &position, Vector3 &resetPosition, Quaternion &parentOrientation, Quaternion &orientation, Quaternion &resetOrientation){
    if(contains(entity)) return;
    
    //Prepare correct collision shape
    btCollisionShape* collisionShape;
    switch(type){
        case component::CollisionType::SPHERE:
            collisionShape = new btSphereShape(collisionArguments[0]);
            break;
        case component::CollisionType::BOX:
            collisionShape = new btBoxShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case component::CollisionType::PLANE:
            collisionShape = new btStaticPlaneShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]), collisionArguments[3]);
            break;
        case component::CollisionType::CYLINDERX:
            collisionShape = new btCylinderShapeX(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case component::CollisionType::CYLINDERY:
            collisionShape = new btCylinderShape(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        case component::CollisionType::CYLINDERZ:
            collisionShape = new btCylinderShapeZ(btVector3(collisionArguments[0], collisionArguments[1], collisionArguments[2]));
            break;
        default:
            return;
    }
    
    //Calculate total position from parent offset
    Matrix4x4 rotMatrix;
    cml::matrix_rotation_quaternion(rotMatrix, parentOrientation);
    Vector3 resultPosition;
    Quaternion resultOrientation;
    resultPosition = transform_vector(rotMatrix, position) + parentPosition;
    resultOrientation = parentOrientation * orientation;
    
    //Prepare motion state
    btTransform temp(btQuaternion(resultOrientation[1], resultOrientation[2], resultOrientation[3], resultOrientation[0]), btVector3(resultPosition[0], resultPosition[1], resultPosition[2]));
    btMotionState* motionState = new DynamicMotionState(temp, entity.getUniqueId(), movable, parentPosition, position, resetPosition, parentOrientation, orientation, resetOrientation);
    
    //Prepare rigid body
    btVector3 inertia;
    collisionShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape, inertia);
    constructionInfo.m_restitution = restitution;
    btRigidBody* rigidBody = new btRigidBody(constructionInfo);
    if(ghost)
        rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
    //Add to all lists
    mEntities.push_back(entity.getUniqueId());
    
    rigidBody->setUserPointer(&entity); //Used in external callback to get reference to entity
    
    mCollisionShapes.push_back(collisionShape);
    mMotionStates.push_back(motionState);
    mRigidBodies.push_back(rigidBody);
    mFlags.push_back(Flag::NONE);
    mInWorld.push_back(false);
    mIsGhost.push_back(ghost);
}

void PhysicsSystem::remove(artemis::Entity& entity){
    if(!contains(entity)) return;
    int index = find(entity.getUniqueId());
    for(HingeConstraint& constraint : mHingeConstraints){
        if(constraint.a == entity.getUniqueId() || constraint.b == entity.getUniqueId()){
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
    mEntities.erase(mEntities.begin() + index);
}

void PhysicsSystem::remove(int index){
    delete mCollisionShapes[index];
    mCollisionShapes.erase(mCollisionShapes.begin() + index);
    delete mMotionStates[index];
    mMotionStates.erase(mMotionStates.begin() + index);
    delete mRigidBodies[index];
    mRigidBodies.erase(mRigidBodies.begin() + index);
    mFlags.erase(mFlags.begin() + index);
    mInWorld.erase(mInWorld.begin() + index);
    mIsGhost.erase(mIsGhost.begin() + index);
}

void PhysicsSystem::start(){
    mRunning = true;
}

void PhysicsSystem::pause(){
    mRunning = false;
}

void PhysicsSystem::clear(){
    removeAllHingeConstraints();
    for(int i = 0; i < mEntities.size(); ++i){
        mDynamicsWorld->removeRigidBody(mRigidBodies[i]);
        remove(i);
        mEntities.erase(mEntities.begin() + i);
        i--;
    }
    mConveyorBelts.clear();
    mCollisionExceptions.clear();
    mMassChanges.clear();
}

void PhysicsSystem::reset(){
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
    for(int i = 0; i < mEntities.size(); ++i){
        static_cast<DynamicMotionState*>(mMotionStates[i])->reset();
        remove(i);
        mEntities.erase(mEntities.begin() + i);
        i--;
    }
    shutdownBullet();
    initBullet();
}

void PhysicsSystem::buildWaterproofRoom(){
    EntityId entFloor = EntitySubSystem::GetInstancePtr()->CreateEntity("Floor");
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloor, new component::Position(0.0,-0.1,0.0));
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloor, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloor, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloor, new component::Physics(0.0, 0.53, component::CollisionType::BOX, std::vector<float>{3, 0.1, 3}, true));
    { //it is not allowed to share a component between serveral entities as artemis calles delete on every component without checking if it realy exists
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entFloor, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entFloor, new component::ManipulateObject(false));
    }
    EntityId entFloorPlane = EntitySubSystem::GetInstancePtr()->CreateEntity("FloorPlane");
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloorPlane, new component::Position(0.0,-0.1,0.0));
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloorPlane, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloorPlane, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entFloorPlane, new component::Physics(0.0, 0.53, component::CollisionType::PLANE, std::vector<float>{0.0,1.0,0.0,0.0}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entFloorPlane, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entFloorPlane, new component::ManipulateObject(false));
    }
    EntityId entRoof = EntitySubSystem::GetInstancePtr()->CreateEntity("Roof");
    EntitySubSystem::GetInstancePtr()->AddComponent(entRoof, new component::Position(0.0,2.954,0.0));
    EntitySubSystem::GetInstancePtr()->AddComponent(entRoof, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entRoof, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entRoof, new component::Physics(0.0, 0.5, component::CollisionType::PLANE, std::vector<float>{0.0,-1.0,0.0,0.0}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entRoof, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entRoof, new component::ManipulateObject(false));
    }
    EntityId entWall1 = EntitySubSystem::GetInstancePtr()->CreateEntity("Wall1");
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall1, new component::Position(-2.0,0.0,0.0));
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall1, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall1, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall1, new component::Physics(0.0, 0.5, component::CollisionType::PLANE, std::vector<float>{1.0,0.0,0.0,0.0}, true));
    
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall1, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall1, new component::ManipulateObject(false));
    }
    EntityId entWall2 = EntitySubSystem::GetInstancePtr()->CreateEntity("Wall2");
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall2, new component::Position(1.7,0.0,0.0));
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall2, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall2, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall2, new component::Physics(0.0, 0.5, component::CollisionType::PLANE, std::vector<float>{-1.0,0.0,0.0,0.0}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall2, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall2, new component::ManipulateObject(false));
    }
    
    EntityId entWall3 = EntitySubSystem::GetInstancePtr()->CreateEntity("Wall3");
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall3, new component::Position(0.0,0.0,-2.0));
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall3, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall3, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall3, new component::Physics(0.0, 0.5, component::CollisionType::PLANE, std::vector<float>{0.0,0.0,1.0,0.0}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall3, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall3, new component::ManipulateObject(false));
    }
    
    EntityId entWall4 = EntitySubSystem::GetInstancePtr()->CreateEntity("Wall4");
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall4, new component::Position(0.0,0.0,1.2));
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall4, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall4, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entWall4, new component::Physics(0.0, 0.5, component::CollisionType::PLANE, std::vector<float>{0.0,0.0,-1.0,0.0}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall4, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entWall4, new component::ManipulateObject(false));
    }
    
    EntityId entTabletop = EntitySubSystem::GetInstancePtr()->CreateEntity("Tabletop");
    EntitySubSystem::GetInstancePtr()->AddComponent(entTabletop, new component::Position(-0.896,1.0,-1.586));
    EntitySubSystem::GetInstancePtr()->AddComponent(entTabletop, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTabletop, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTabletop, new component::Physics(0.0, 0.68, component::CollisionType::BOX, std::vector<float>{1.08,0.055,0.35}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entTabletop, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entTabletop, new component::ManipulateObject(false));
    }
    
    EntityId entTableLeg1 = EntitySubSystem::GetInstancePtr()->CreateEntity("TableLeg1");
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg1, new component::Position(-1.829, 0.463, -1.817));
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg1, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg1, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg1, new component::Physics(0.0, 0.68, component::CollisionType::BOX, std::vector<float>{0.05,0.49,0.05}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg1, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg1, new component::ManipulateObject(false));
    }
    
    EntityId entTableLeg2 = EntitySubSystem::GetInstancePtr()->CreateEntity("TableLeg2");
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg2, new component::Position(0.052, 0.463, -1.817));
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg2, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg2, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg2, new component::Physics(0.0, 0.68, component::CollisionType::BOX, std::vector<float>{0.05,0.49,0.05}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg2, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg2, new component::ManipulateObject(false));
    }
    
    EntityId entTableLeg3 = EntitySubSystem::GetInstancePtr()->CreateEntity("TableLeg3");
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg3, new component::Position(0.052, 0.463, -1.374));
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg3, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg3, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg3, new component::Physics(0.0, 0.68, component::CollisionType::BOX, std::vector<float>{0.05,0.49,0.05}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg3, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg3, new component::ManipulateObject(false));
    }
    
    EntityId entTableLeg4 = EntitySubSystem::GetInstancePtr()->CreateEntity("TableLeg4");
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg4, new component::Position(-1.829, 0.463, -1.374));
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg4, new component::Orientation());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg4, new component::Scaling());
    EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg4, new component::Physics(0.0, 0.68, component::CollisionType::BOX, std::vector<float>{0.05,0.49,0.05}, true));
    {
        component::RenderModel* renderComp = new component::RenderModel();
        renderComp->visible = false;
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg4, renderComp);
        EntitySubSystem::GetInstancePtr()->AddComponent(entTableLeg4, new component::ManipulateObject(false));
    }
}

void PhysicsSystem::applyForce(EntityId entityId, Vector3 force, Vector3 offsetFromCOMWorldSpace){
    int index = find(entityId);
    if(index == -1)
        return;
    mRigidBodies[index]->activate();
    mRigidBodies[index]->applyImpulse(btVector3(force[0], force[1], force[2]), btVector3(offsetFromCOMWorldSpace[0], offsetFromCOMWorldSpace[1], offsetFromCOMWorldSpace[2]));
}

void PhysicsSystem::applyTorque(EntityId entityId, Vector3 torque){
    int index = find(entityId);
    if(index == -1)
        return;
    mRigidBodies[index]->activate();
    mRigidBodies[index]->applyTorqueImpulse(btVector3(torque[0], torque[1], torque[2]));
}

void PhysicsSystem::setMass(EntityId entityId, double mass){
    int index = find(entityId);
    if(index == -1) return;
    mMassChanges.push_back(MassChange{index, mass});
}

void PhysicsSystem::applyExplosion(EntityId bomb, Vector3 bombPos, double maxPower){
    double maxDist = sqrt(maxPower) - 1;
    //Iterate over all entities, check distance limit, if close enough apply force
    for(int i = 0; i < mEntities.size(); ++i){
        if(mEntities[i] == bomb) continue;
        artemis::Entity* entity = EntitySubSystem::GetInstancePtr()->getEntity(mEntities[i]);
        Vector3 objPos = ((component::Position*) entity->getComponent(artemis::ComponentTypeManager::getTypeFor<component::Position>()))->position;
        Vector3 dir = objPos - bombPos;
        double dist = dir.length();
        if(dist > maxDist) continue;
        dir /= dist;
        dir *= maxPower / ((1 + dist) * (1 + dist));
        mRigidBodies[i]->activate();
        mRigidBodies[i]->applyImpulse(btVector3(dir[0], dir[1], dir[2]), btVector3(0.0, 0.0, 0.0));
    }
}

void PhysicsSystem::addConveyorBelt(EntityId belt, Vector3 force){
    int index = find(belt);
    if(index == -1 || mRigidBodies[index]->getInvMass() != 0.0)
        return;
    mRigidBodies[index]->setLinearVelocity(btVector3(force[0], force[1], force[2])); //linear velocity of static objects is basically a variable, can be used to contain power of a static conveyorbelt
    mRigidBodies[index]->setCollisionFlags(mRigidBodies[index]->getCollisionFlags()  | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    mConveyorBelts.insert(belt);
    //std::cout << "Belt id: "<<belt<<std::endl;
}

void PhysicsSystem::removeConveyorBelt(EntityId belt){
    if(!isConveyorBelt(belt))
        return;
    int index = find(belt);
    if(index == -1 || mRigidBodies[index]->getInvMass() != 0.0)
        return;
    mRigidBodies[index]->setLinearVelocity(btVector3(0.0, 0.0, 0.0));
    mRigidBodies[index]->setCollisionFlags(mRigidBodies[index]->getCollisionFlags()  & ~btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    mConveyorBelts.erase(belt);
}

bool PhysicsSystem::isConveyorBelt(EntityId belt){
    return mConveyorBelts.find(belt) != mConveyorBelts.end();
}

btVector3 PhysicsSystem::getLinearVelocity(EntityId belt){
    if(!isConveyorBelt(belt))
        return btVector3(0.0, 0.0, 0.0);
    int index = find(belt);
    return mRigidBodies[index]->getLinearVelocity();
}

bool PhysicsSystem::isGhostObject(EntityId ghost){
    int index = find(ghost);
    if(index == -1)
        return false;
    return mIsGhost[index];
}

void PhysicsSystem::addCollisionException(EntityId a, EntityId b){
    mCollisionExceptions.insert(PreventCollisionPair(a, b));
    mCollisionExceptions.insert(PreventCollisionPair(b, a));
}

void PhysicsSystem::removeCollisionException(EntityId a, EntityId b){
    mCollisionExceptions.erase(PreventCollisionPair(a, b));
    mCollisionExceptions.erase(PreventCollisionPair(b, a));
}

void PhysicsSystem::addHingeConstraint(EntityId a, Vector3 pivotA, Vector3 axisA, EntityId b, Vector3 pivotB, Vector3 axisB, double low, double high){
    for(HingeConstraint& constraint : mHingeConstraints){
        if(constraint.a == a && constraint.b == b){
            LOG("You can't add another hinge constraint between " << a << " and " << b << ". Please remove the old one first.");
            return;
        }
    }
    mHingeConstraints.push_back(HingeConstraint{a, pivotA, axisA, b, pivotB, axisB, low, high, false, nullptr});
}

void PhysicsSystem::removeHingeConstraint(EntityId a, EntityId b){
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

void PhysicsSystem::removeHingeConstraint(EntityId a){
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

void PhysicsSystem::setHingeConstraintLimits(EntityId a, EntityId b, double low, double high){
    for(HingeConstraint& c : mHingeConstraints){
        if(c.a == a && c.b == b && c.constraintObject != nullptr){
            c.constraintObject->setLimit(low * M_PI / 180.0, high * M_PI / 180.0, HINGE_SOFTNESS, HINGE_BIAS, HINGE_RELAXATION);
        }
    }
}

void customNearCallback(btBroadphasePair& collisionPair, btCollisionDispatcher& dispatcher, const btDispatcherInfo& dispatchInfo){
    
    EntityId a = ((artemis::Entity*)((btCollisionObject*)collisionPair.m_pProxy0->m_clientObject)->getUserPointer())->getUniqueId();
    EntityId b = ((artemis::Entity*)((btCollisionObject*)collisionPair.m_pProxy1->m_clientObject)->getUserPointer())->getUniqueId();
    
    if(PhysicsSystem::GetInstancePtr()->getCollisionExceptions().find(PreventCollisionPair(a, b)) != PhysicsSystem::GetInstancePtr()->getCollisionExceptions().end()){
        return;
    }
    
    dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
}

bool CustomMaterialCombinerCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1)
{
    EntityId a = ((artemis::Entity*)(colObj0->getCollisionObject())->getUserPointer())->getUniqueId();
    EntityId b = ((artemis::Entity*)(colObj1->getCollisionObject())->getUserPointer())->getUniqueId();
    
    bool convA = PhysicsSystem::GetInstancePtr()->isConveyorBelt(a);
    bool convB = PhysicsSystem::GetInstancePtr()->isConveyorBelt(b);
    
    //Find out which entity is the belt, so direction can be set correctly
    btVector3 dir;
    if(convA){
        if(convB)
            return true;
        dir = PhysicsSystem::GetInstancePtr()->getLinearVelocity(a);
    } else if(convB){
        dir = PhysicsSystem::GetInstancePtr()->getLinearVelocity(b);
    }
    
    cp.m_lateralFrictionInitialized = true;
    ///choose a target velocity in the friction dir1 direction, for a conveyor belt effect
    cp.m_lateralFrictionDir1.setValue(dir[0], dir[1], dir[2]);
    //cp.m_lateralFrictionDir1.normalize();
    
    ///optionally downscale the friction direction 2 for lower (anisotropic) friction (rather than a unit vector)
    btScalar downscale = 1.f;
    cp.m_lateralFrictionDir2 = downscale*cp.m_lateralFrictionDir1.cross(cp.m_normalWorldOnB);
    
    cp.m_contactMotion1 = 1.f;
    cp.m_contactCFM2 = 0.1;
    cp.m_combinedFriction = 1;
    //cp.m_combinedRestitution = calculateCombinedRestitution(restitution0,restitution1);
    return true;
}

void customTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    int numManifolds = world->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++)
    {
        btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        if(contactManifold->getNumContacts() == 0) continue; // Filter "almost-collisions"
        EntityId a = ((artemis::Entity*)contactManifold->getBody0()->getUserPointer())->getUniqueId();
        EntityId b = ((artemis::Entity*)contactManifold->getBody1()->getUserPointer())->getUniqueId();
        bool ghostA = PhysicsSystem::GetInstancePtr()->isGhostObject(a);
        bool ghostB = PhysicsSystem::GetInstancePtr()->isGhostObject(b);
        
        if(ghostA){
            if(ghostB)
                continue;
            GameLogicSubSystem::GetInstancePtr()->TriggerFiredCallback(a, b);
        } else if(ghostB){
            GameLogicSubSystem::GetInstancePtr()->TriggerFiredCallback(b, a);
        }
    }
}
ENDNAMESPACE
*/
