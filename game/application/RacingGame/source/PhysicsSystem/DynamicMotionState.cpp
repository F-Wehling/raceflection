#include <stdio.h>
#include "PhysicsSystem/DynamicMotionState.h"

BEGINNAMESPACE
/*
DynamicMotionState::DynamicMotionState(const btTransform &initialPosition, int entity, bool movable, Vector3 &parentPos, Vector3 &pos, Vector3 &resetPos, Quaternion &parentOri, Quaternion &ori, Quaternion &resetOri) : mInitialPosition(initialPosition), mEntity(entity), movable(movable), mParentPos(&parentPos), mPos(&pos), mResetPos(&resetPos), mParentOri(&parentOri), mOri(&ori), mResetOri(&resetOri) {}

void DynamicMotionState::setEntity(int entity){
    mEntity = entity;
}

void DynamicMotionState::getWorldTransform(btTransform &worldTrans) const {
    worldTrans = mInitialPosition;
}

void DynamicMotionState::setWorldTransform(const btTransform &worldTrans){
    if(mEntity == -1.0) return;
    //Gets new values
    btQuaternion rot = worldTrans.getRotation();
    btVector3 pos = worldTrans.getOrigin();
    
    Vector3 resultPosition = Vector3(pos.getX(), pos.getY(), pos.getZ());
    Quaternion resultOrientation = Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
    
    //Calculate inverse rotation from parent orientation
    Matrix4x4 inverseRotMatrix;
    cml::matrix_rotation_quaternion(inverseRotMatrix, *mParentOri);
    inverseRotMatrix.transpose();
    
    //Transform new values according to parent offset
    Vector3 outPos = transform_vector(inverseRotMatrix, (resultPosition - *mParentPos));
    Quaternion inverseParentOri = *mParentOri;
    inverseParentOri.inverse();
    Quaternion outOri = inverseParentOri * resultOrientation;
    
    *mPos = outPos;
    *mOri = outOri;
}

void DynamicMotionState::reset(){
    if(movable) return;
    *mPos = *mResetPos;
    *mOri = *mResetOri;
}
*/
ENDNAMESPACE
