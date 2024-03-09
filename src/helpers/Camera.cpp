//
// Created by ninja on 3/7/2024.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float pitch, float yaw)
    : m_cameraPos(position), m_cameraUp(up), m_pitch(pitch), m_yaw(yaw)
{

}

float Camera::getPitch() const {return m_pitch;};
float Camera::getYaw() const {return m_yaw;};

glm::vec3 Camera::getCameraDir()
{
    updateVectors();
    return m_cameraDir;
};

glm::vec3 Camera::getCameraPos()
{
    return m_cameraPos;
};

glm::vec3 Camera::getCameraRight()
{
    updateVectors();
    return m_cameraRight;
};

void Camera::setPitch(float pitch) {
    if(pitch > -89 && pitch < 89)
    {
        m_pitch = pitch;
        updateVectors();
    }
};
void Camera::setYaw(float yaw)
{
    m_yaw = yaw;

    if(m_yaw > 360)
    {
        m_yaw = m_yaw - 360;
    }
    if(m_yaw < 0)
    {
        m_yaw = 360 + m_yaw;
    }

    updateVectors();
};

void Camera::changeYaw(float yawOffset)
{
    m_yaw += yawOffset;

    if(m_yaw > 360)
    {
        m_yaw = m_yaw - 360;
    }
    if(m_yaw < 0)
    {
        m_yaw = 360 + m_yaw;
    }

    updateVectors();
};
void Camera::changePitch(float pitchOffset) {
    m_pitch += pitchOffset;
    if(m_pitch > 89.0)
    {
        m_pitch = 89.0f;
    }
    if(m_pitch < -89)
    {
        m_pitch = -89;
    }

    updateVectors();
};

void Camera::setCameraPos(const glm::vec3 &newPos) {m_cameraPos = newPos;};
void Camera::changeCameraPos(const glm::vec3 &posOffset) {m_cameraPos += posOffset;};

void Camera::updateVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraDir = glm::normalize(direction);

    m_cameraRight = glm::normalize(glm::cross(m_cameraDir, worldUp));
    m_cameraUp = glm::normalize(glm::cross(m_cameraDir, m_cameraRight));
}

const glm::mat4& Camera::getView()
{
    return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraDir, m_cameraUp);
}



