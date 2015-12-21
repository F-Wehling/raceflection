#pragma once

#include <LinearMath/btMotionState.h>

class btTransform;

BEGINNAMESPACE

class ObjectSystem;

using GameObjectId = int;

//Standard MotionState for our physical objects
//This MotionState gives the object position to the physics and receives updated positions.
//The new position is then delegated to the object for the rendering to update.
class DynamicMotionState : public btMotionState
{
private:
    ObjectSystem *mObjectSystem;

    GameObjectId mGameObjectId = -1.0;
    btTransform mCurrentPosition;

public:
    //Constructor, simply saves all arguments into the variables
    DynamicMotionState(ObjectSystem* objectSystem, const btTransform &initialPosition, GameObjectId gameObjectId);
    virtual ~DynamicMotionState(){}
    
    void setGameObject(GameObjectId gameObjectId);
    //Returns the current position, called by Bullet once when the object is added and on each simulation step
    virtual void getWorldTransform(btTransform &worldTrans) const;
    //Sets the world transform of the entity. Called by Bullet after each position update.
    virtual void setWorldTransform(const btTransform &worldTrans);
    //Sets he current position of the object. Used for objects that are moved without involvement of physics (for example, moving the car back to the start)
    void setCurrentPos(const btTransform &currentPosition);
};

ENDNAMESPACE
