#include "RenderSystem/Camera.h"

#include "ObjectSystem/GameObject.h"

//use ACGls camera
#include <ACGL/Scene/GenericCamera.hh>
BEGINNAMESPACE
namespace cam = ACGL::Scene;
Camera::Camera(){
    new(m_CameraStorage) cam::GenericCamera();
}

glm::mat4x4 Camera::getViewMatrix() const {
    const cam::GenericCamera* cb = (const cam::GenericCamera*)m_CameraStorage;
    return cb->getViewMatrix();
}

glm::mat4x4 Camera::getProjectionMatrix() const {
    const cam::GenericCamera* cb = (const cam::GenericCamera*)m_CameraStorage;
    return cb->getProjectionMatrix();
}

glm::ivec2 Camera::getViewportSize() const {
    const cam::GenericCamera* cb = (const cam::GenericCamera*)m_CameraStorage;
    return cb->getViewportSize();
}

glm::vec2 Camera::getClippingPlanes() const
{
	const cam::GenericCamera* cb = (const cam::GenericCamera*)m_CameraStorage;
	return glm::vec2(cb->getNearClippingPlane(), cb->getFarClippingPlane());
}

void Camera::update() {
    if(!m_BelongTo) return;

    cam::GenericCamera* genCam = (cam::GenericCamera*) m_CameraStorage;
    genCam->setLookAtMatrix(m_BelongTo->getPosition(), m_BelongTo->getPosition() + m_BelongTo->getForward(), m_BelongTo->getUp());
}

void Camera::setViewportSize(glm::uvec2 vec){;
    cam::GenericCamera* genCam = (cam::GenericCamera*)m_CameraStorage;
    genCam->resize(vec.x, vec.y);
}

void Camera::setVerticalFieldOfView(float _fovv){
    cam::GenericCamera* genCam = (cam::GenericCamera*)m_CameraStorage;
    genCam->setVerticalFieldOfView(_fovv);
}

ENDNAMESPACE
