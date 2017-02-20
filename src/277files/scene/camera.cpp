#include "camera.h"

#include <la.h>

#ifndef OLD_CAMERA
Camera::Camera():
    Camera(531, 811)
{
    look = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp)
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}


void Camera::RecomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);

    float tan_fovy = tan(glm::radians(fovy/2));
    float len = glm::length(ref - eye);
    aspect = width/height;
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

void Camera::RotateAboutUp(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), up);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg)
{
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), right);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
    ref += translation;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}

#else

Camera::Camera() {}

Camera::Camera(int w, int h)
{
    theta = -45 * DEG2RAD;
    phi = -45 * DEG2RAD;
    zoom = 100;
    fovy = 45 * DEG2RAD;
    width = w;
    height = h;
    near_clip = 0.1f;
    far_clip = 1000;

    eye = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom)) * glm::vec4(0, 0, 0, 1);
    ref = glm::vec4(0, 0, 0, 1);
    up = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::vec4(0, 1, 0, 0);
}

void Camera::RecomputeEye()
{
    eye = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom)) * glm::vec4(0, 0, 0, 1);
    up = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::vec4(0, 1, 0, 0);
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(fovy, width / height, near_clip, far_clip) * glm::lookAt(glm::vec3(eye), glm::vec3(ref), glm::vec3(up));
}
#endif
