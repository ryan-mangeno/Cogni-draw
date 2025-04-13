#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"


class Camera
{
public:


	glm::mat4 CalcViewMatrix() const;
	glm::mat4 CalcMVP(const glm::mat4& model_mat = glm::mat4(1.0f)) const;

	inline glm::vec3& GetPos() { return m_Position; }
	inline glm::mat4& GetProjMat() { return m_ProjectionMat; }
	inline float GetZoom() const { return m_Zoom; }

	void DispatchKeyboardEvent(MovementDir direction, float deltaTime);
	void DispatchMouseMoveEvent(float xrot, float yrot);
	void DispatchMouseScrollEvent(float scroll);

	void OnUpdate(float deltaTime);

	// singleton for camera, only one camera for model viewer
	inline static Camera& GetCamera() 
	{
		static Camera c(glm::vec3(0.0f, 25.0f, 0.0f), 1920, 1080, 45.f, 0.1f, 100.f);
		return c;
	}

private:


	// so we can construct one for singleton but not make more
	Camera(glm::vec3 position, int width, int height, float fov, float nearPlane, float farPlane);

	glm::mat4 m_ProjectionMat;

	glm::vec3 m_Position;
	glm::vec3 m_Orientation;
	glm::vec3 m_Right;
	glm::vec3 m_Up;

	float m_Yaw;
	float m_Pitch;

	unsigned int m_Width;
	unsigned int m_Height;

	float m_Speed;
	float m_Velocity;
	float m_Sensitivity;
	float m_Zoom;

	void updateCameraVectors();
};


#endif