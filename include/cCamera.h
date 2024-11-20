#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class cTransform
{
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    cTransform()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    glm::vec3 getPosition() { return position; }
    glm::vec3 getRotation() { return rotation; }
    glm::vec3 getScale() { return scale; }

    void setPosition(glm::vec3 pos) { position = pos; }
    void setRotation(glm::vec3 rot) { rotation = rot; }
    void setScale(glm::vec3 scl) { scale = scl; }
};

class cCamera: public cTransform
{
private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

public:
    cCamera()
    {
        viewMatrix = glm::mat4(1.0f);
        projectionMatrix = glm::mat4(1.0f);
        fov = 60.0f;
        aspectRatio = 1.0f;
        nearPlane = 0.1f;
        farPlane = 100.0f;
    }

    void calculateViewMatrix()
    {

        glm::mat4 rotMatrix = glm::mat4(1.0f);
        rotMatrix = glm::rotate(rotMatrix, glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
        rotMatrix = glm::rotate(rotMatrix, glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotMatrix = glm::rotate(rotMatrix, glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::vec3 forward = glm::vec3(rotMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
        glm::vec3 up = glm::vec3(rotMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        viewMatrix = glm::lookAt(getPosition(), getPosition() + forward, up);

        // viewMatrix = glm::lookAt(getPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // getPosition() + 
    }

    void calculateProjectionMatrix()
    {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    glm::mat4 getViewMatrix() { return viewMatrix; }
    glm::mat4 getProjectionMatrix() { return projectionMatrix; }
    float getFov() { return fov; }
    float getAspectRatio() { return aspectRatio; }
    float getNearPlane() { return nearPlane; }
    float getFarPlane() { return farPlane; }

    void setViewMatrix(glm::mat4 view) { viewMatrix = view; }
    void setProjectionMatrix(glm::mat4 proj) { projectionMatrix = proj; }
    void setFov(float f) { fov = f; }
    void setAspectRatio(float ar) { aspectRatio = ar; }
    void setNearPlane(float np) { nearPlane = np; }
    void setFarPlane(float fp) { farPlane = fp; }
};