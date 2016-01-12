#include "ObjectSystem/ObjectSystem.h"
#include "ObjectSystem/GameObjectComponent.h"

#include "PackageSpec.h"
#include "PhysicsSpec.h"
#include "GameObjectSpec.h"
#include "PhysicsSystem/PhysicsSystem.h"

#include "Configuration/ConfigSettings.h"

#include "Multithreading/JobScheduler.h"
#include "Multithreading/ParallelFor.h"

#include <map>

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
    std::map<UUID, const PhysicsSpec*> physicsSpecs;

    for(size_t i = 0; i < pkgSpec->getPhysicsCount(); i++ ){
        const PhysicsSpec* spec = pkgSpec->getPhysicsSpec(i);
        physicsSpecs[spec->uuid] = spec;
    }

    PhysicsSystem* phySys = mMain->getPhysicsSystemPtr();
    for(size_t i = 0; i < pkgSpec->getGameObjectCount(); i++){
        const GameObjectSpec* spec = pkgSpec->getGameObjectSpec(i);
        GameObject* obj = createObject();
        obj->setPosition(glm::vec3(spec->position[0], spec->position[1], spec->position[2]));
        obj->setRotation(glm::quat(spec->rotation[0], spec->rotation[1], spec->rotation[2], spec->rotation[3]));
        obj->setScaling(glm::vec3(spec->scaling[0], spec->scaling[1], spec->scaling[2]));
        obj->setFlags(GameObjectFlags::Enum(spec->flags));

        if(obj->hasPhysics()){
            const PhysicsSpec* phySpec = physicsSpecs[spec->physicsId];
            phySys->add(obj,CollisionType::Enum( phySpec->collisionType ), phySpec->collisionShapeDataFloat32, phySpec->mass, phySpec->restitution );
            phySys->activate(obj->getID());
        }

        if(obj->hasMesh()){

        }
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
