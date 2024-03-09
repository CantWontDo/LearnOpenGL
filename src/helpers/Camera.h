//
// Created by ninja on 3/7/2024.
//

#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:

    explicit Camera(glm::vec3 position = glm::vec3(0, 0, -3), glm::vec3 up = glm::vec3(0, 1, 0),
            float pitch = 0, float yaw = 270);

    const glm::mat4& getView();

    void setYaw(float yaw);
    void setPitch(float pitch);

    [[nodiscard]] float getYaw() const;
    [[nodiscard]] float getPitch() const;

    void changeYaw(float yawOffset);
    void changePitch(float pitchOffset);

    void setCameraPos(const glm::vec3& newPos);
    void changeCameraPos(const glm::vec3& posOffset);

    [[nodiscard]] glm::vec3 getCameraPos();
    [[nodiscard]] glm::vec3 getCameraDir();
    [[nodiscard]] glm::vec3 getCameraRight();

    glm::vec3 worldFront = glm::vec3(0, 0, -1);
    glm::vec3 worldRight = glm::vec3(1, 0, 0);
    glm::vec3 worldLeft = glm::vec3(-1, 0, 0);
    glm::vec3 worldBack = glm::vec3(0, 0, 1);
    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    glm::vec3 worldDown = glm::vec3(0, -1, 0);

private:
    // z values are negative forward and positive backward
    glm::vec3 m_cameraDir;
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraRight;

    void updateVectors();

    float m_yaw = 0;
    float m_pitch = -90;
};


#endif //LEARNOPENGL_CAMERA_H
