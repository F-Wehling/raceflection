#include "ObjectSystem/ObjectSystem.h"
#include "ObjectSystem/GameObjectComponent.h"

#include "PackageSpec.h"
#include "PhysicsSpec.h"
#include "PhysicsSystem/PhysicsSystem.h"

#include "Configuration/ConfigSettings.h"

#include "Multithreading/JobScheduler.h"
#include "Multithreading/ParallelFor.h"

BEGINNAMESPACE

ConfigSettingUint32 cfgMaxGameObjects("maxGameObjects","Specifies the maximum number of GameObjects",1000);

ObjectSystem::ObjectSystem(Main* main):mMain(main),mGameObjects(cfgMaxGameObjects), mNumGameObjects(0)
{
}

ObjectSystem::~ObjectSystem()
{
    //delete mGameObjects;
}

GameObject* ObjectSystem::getObjectByID(GameObjectID ID)
{
    return &mGameObjects[ID];
}

void ObjectSystem::deleteObject(GameObjectID ID){
    std::swap(mGameObjects[ID], mGameObjects[--mNumGameObjects]);
}

bool ObjectSystem::isTriggerArea(GameObjectID ID)
{
    return getObjectByID(ID)->isTriggerArea();
}

bool ObjectSystem::createObjectsFromPackageSpec(PackageSpec * pkgSpec)
{

    for(size_t i = 0; i < pkgSpec->getPhysicsCount(); i++ ){

        const PhysicsSpec* spec = pkgSpec->getPhysicsSpec(i);
        PhysicsSystem* phySys = mMain->getPhysicsSystemPtr();
        GameObject* obj = createObject();

        phySys->add(*obj,CollisionType::Enum( spec->collisionType ), spec->collisionShapeDataFloat32,spec->mass, spec->restitution );

    }

	return true;
}

struct ObjSysPForParam {
    ObjectSystem* instance;
    float32 dt;
};

bool ObjectSystem::tick(float32 dt){
    ObjSysPForParam param = {this, dt};
    Job* pfor = parallel_for(getFirstObject(), mNumGameObjects, &ObjectSystem::tickGameObjects, (void*)&param);
    JobScheduler::Wait(pfor);
    return true;
}

void ObjectSystem::tickGameObjects(GameObject *gameObjects, uint32 numObjects, void *extraInfo){
    ObjSysPForParam* param = (ObjSysPForParam*)extraInfo;
    ObjectSystem* objSys = param->instance;
    for( uint32 i = 0; i < numObjects; ++i){
        objSys->tickGameObject(gameObjects + i, param->dt);
    }
}

void ObjectSystem::tickGameObject(GameObject *go, float32 dt){
    for(uint32 i = 0; i < go->mNumComponents; ++i){
        go->mComponents[i]->process(dt, go);
    }
}

ENDNAMESPACE
