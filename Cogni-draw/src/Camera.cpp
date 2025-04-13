#include "Camera.h"

//general up direction for projection stuff 
constexpr glm::vec3 g_Up(0.f, 1.f, 0.f);

#include <GLFW/glfw3.h>


Camera::Camera(glm::vec3 position, int width, int height, float fov, float nearPlane, float farPlane)
    : m_Width(width),
    m_Height(height),
    m_Position(position),
    m_Orientation(0.0f, 0.0f, -1.0f),
    m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
    m_Yaw(-90.f),
    m_Pitch(0.f),
    m_Zoom(75.0f),
    m_Speed(20.0f),
    m_Velocity(30.0f),
    m_Sensitivity(0.5f)
{                                       
    m_ProjectionMat = glm::perspective(glm::radians(m_Zoom), float(m_Width) / float(m_Height), nearPlane, farPlane);
    updateCameraVectors();
}

glm::mat4 Camera::CalcViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Orientation, g_Up);
}

glm::mat4 Camera::CalcMVP(const glm::mat4& model_mat) const
{
    return m_ProjectionMat * CalcViewMatrix() * model_mat;
}

void Camera::DispatchKeyboardEvent(int key, float deltaTime)
{

    float velocity = m_Speed * deltaTime;

    switch (key) 
    {

        case GLFW_KEY_W:      m_Position += m_Orientation * velocity; break;
        case GLFW_KEY_S:         m_Position += -m_Orientation * velocity; break;
        case GLFW_KEY_A:         m_Position += -m_Right * velocity; break;
        case GLFW_KEY_D:        m_Position += m_Right * velocity; break;
        case GLFW_KEY_Q:           m_Position += -g_Up * velocity; break;
        case GLFW_KEY_E:         m_Position += g_Up * velocity; break;

        default:                                           break;

    }
}

void Camera::OnUpdate(float deltaTime)
{
}

void Camera::DispatchMouseMoveEvent(float xrot, float yrot)
{

    xrot *= m_Sensitivity;
    yrot *= m_Sensitivity;

    m_Yaw += xrot;
    m_Pitch += yrot;


    if (!(m_Pitch >= -85.0f && m_Pitch <= 85.0f)) 
    {
        m_Pitch -= yrot;
    }


    updateCameraVectors();
}

void Camera::DispatchMouseScrollEvent(float scroll)
{
    m_Speed += scroll;
}

void Camera::updateCameraVectors()
{
    glm::clamp(m_Yaw, -180.f, 180.f);

    m_Orientation.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Orientation.y = sin(glm::radians(m_Pitch));
    m_Orientation.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Orientation = glm::normalize(m_Orientation);

    // ori changed to we need to recalc up and right
    m_Right = glm::normalize(glm::cross(m_Orientation, g_Up));
    m_Up = glm::normalize(glm::cross(m_Right, m_Orientation));
}