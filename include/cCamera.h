#pragma once

#include <cGameObject.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class cCamera: public cGameObject
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
        auto transform = getTransform();

        glm::mat4 rotMatrix = glm::mat4(1.0f);
        rotMatrix = glm::rotate(rotMatrix, glm::radians(transform.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
        rotMatrix = glm::rotate(rotMatrix, glm::radians(transform.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotMatrix = glm::rotate(rotMatrix, glm::radians(transform.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::vec3 forward = glm::vec3(rotMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
        glm::vec3 up = glm::vec3(rotMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        viewMatrix = glm::lookAt(transform.getPosition(), transform.getPosition() + forward, up);

        // viewMatrix = glm::lookAt(getPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // getPosition() + 
    }

    void calculateProjectionMatrix()
    {
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    //get transform ref
    cTransform &getTransform() { return cGameObject::getTransform(); }

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