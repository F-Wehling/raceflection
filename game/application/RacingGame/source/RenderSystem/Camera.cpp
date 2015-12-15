#include "RenderSystem/Camera.h"

#include "ObjectSystem/GameObject.h"

//use ACGls camera
#include <ACGL/Scene/CameraBase.hh>
#include <ACGL/Scene/FixedCamera.hh>
#include <ACGL/Scene/GenericCamera.hh>
#include <ACGL/Scene/OculusRiftCamera.hh>
BEGINNAMESPACE
namespace cam = ACGL::Scene;
Camera::Camera(CameraType type /* = GENERIC */) : m_Type(type){
    cam::CameraBase* camera;
    switch(type) {
    case FIXED:
        camera = new(m_CameraStorage) cam::FixedCamera();
        break;
    case GENERIC:
        camera = new(m_CameraStorage) cam::GenericCamera();
        break;
    }
}

glm::mat4x4 Camera::getViewMatrix() const {
    const cam::CameraBase* cb = (cam::CameraBase*)m_CameraStorage;
    return cb->getViewMatrix();
}

glm::mat4x4 Camera::getPerspectiveMatrix() const {
    const cam::CameraBase* cb = (cam::CameraBase*)m_CameraStorage;
    return cb->getViewMatrix();
}

glm::uvec2 Camera::getViewportSize() const {
    const cam::CameraBase* cb = (cam::CameraBase*)m_CameraStorage;
    return cb->getViewportSize();
}

void Camera::update() {
    if(!m_BelongTo) return;

    switch(m_Type){
    case FIXED: return;
    case GENERIC:
    {
        cam::GenericCamera* genCam = (cam::GenericCamera*) m_CameraStorage;
        genCam->setPosition(m_BelongTo->getPosition());
        genCam->setRotationMatrix(glm::mat3_cast(m_BelongTo->getRotation()));
    }
    }

}

ENDNAMESPACE
